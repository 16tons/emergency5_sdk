// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/spatial/SpatialPartition2D.h"
#include "qsf/component/spatial/SpatialPartition2DBruteForceLookup.h"
#include "qsf/base/manager/PagedAllocator.h"

#include <glm/glm.hpp>

#include <map>
#include <set>
#include <algorithm>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    2D quadtree spatial partition implementation
	*
	*  @remarks
	*    The quadtree partition imposes additional constraints on the ItemTraits:
	*
	*    Split up a bounding volume into four quadrants:
	*    static void ItemTraits::splitBoundsQuadrant(ItemTraits::Bounds bounds, uint32 quadrant, ItemTraits::Bounds& outQuadrantBounds)
	*
	*    Check if bounds are completely encompassed by other bounds (i.e. innerBounds is completely inside outerBounds)
	*    static bool ItemTraits::areBoundsEncompassingBounds(ItemTraits::Bounds outerBounds, ItemTraits::Bounds innerBounds)
	*/
	template<class ItemType, class SpatialItemTraits>
	class SpatialPartition2DQuadtree : public SpatialPartition2D<ItemType, SpatialItemTraits>
	{
	public:
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::Item Item;
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::ItemIdentity ItemIdentity;
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::ItemBounds ItemBounds;
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::ItemSet ItemSet;
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::ItemTraits ItemTraits;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		SpatialPartition2DQuadtree(const ItemBounds& worldBounds, uint32 maxDepth = 10) : mNodeAlloc(256), mWrappedItemAlloc(1024), mMaxDepth(maxDepth)
		{
			mRoot.bounds = worldBounds;
		}


	//[-------------------------------------------------------]
	//[ Public virtual SpatialPartition2D<> methods           ]
	//[-------------------------------------------------------]
	public:
		virtual void lookUpElementsInRange(const ItemBounds& bounds, ItemSet& outFoundObjects) override
		{
			traverseNode(mRoot, bounds, outFoundObjects);

			// We have to also look in the "out of bounds" objects
			mOutOfWorldItems.lookUpElementsInRange(bounds, outFoundObjects);
		}

		template<typename AnyBounds>
		void lookUpElementsInAnyRange(const AnyBounds& bounds, ItemSet& outFoundObjects)
		{
			traverseNode(mRoot, bounds, outFoundObjects);

			// We have to also look in the "out of bounds" objects
			mOutOfWorldItems.lookUpElementsInAnyRange(bounds, outFoundObjects);
		}

		virtual bool add(const Item& item) override
		{
			std::pair<typename ItemWrapMap::iterator, bool> insertResult = mWrappedItems.emplace(this->getItemIdentity(item), nullptr);

			if (insertResult.second)
			{
				WrappedItem* wrappedItem = allocWrappedItem();
				wrappedItem->item = item;
				wrappedItem->bounds = this->getItemBounds(item);
				wrappedItem->next = nullptr;
				wrappedItem->containedNode = nullptr;

				insertResult.first->second = wrappedItem;

				linkItemIntoTree(wrappedItem);

				return true;
			}

			return false;
		}

		virtual bool remove(const ItemIdentity& itemIdentity) override
		{
			typename ItemWrapMap::iterator found = mWrappedItems.find(itemIdentity);

			if (found != mWrappedItems.end())
			{
				WrappedItem* wrappedItem = found->second;
				mWrappedItems.erase(found);
				unlinkItemFromTree(wrappedItem);
				deallocWrappedItem(wrappedItem);

				return true;
			}

			return false;
		}

		virtual void update(const ItemIdentity& itemIdentity) override
		{
			WrappedItem* wrappedItem = mWrappedItems[itemIdentity];

			// Update bounds
			wrappedItem->bounds = this->getItemBounds(wrappedItem->item);

			// TODO(oi) Improve speed by using potential spatial coherence, e.g. less needed tree
			// traversals. May need restructuring of tree (i.e. storage of parent nodes)
			unlinkItemFromTree(wrappedItem);
			linkItemIntoTree(wrappedItem);
		}


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Node;
		struct WrappedItem;

		/**
		*  @brief
		*    Item structure which is used to link real items into the
		*    tree. Especially as this structure serves as a intrusive
		*    linked-list of items which are in a specific node and that
		*    items need to know in which node they are, specifically
		*/
		struct WrappedItem
		{
			WrappedItem* next;		///< When attached to a node, this is the next item which is in this node
			Node* containedNode;	///< The node this item is currently attached to
			Item item;				///< Storage for the generic item data
			ItemBounds bounds;		///< Cached bounds for item
		};

		/**
		*  @brief
		*    Structure which describes each node in the quadtree
		*    Nodes are always square and described by the center and half width (box radius)
		*/
		struct Node
		{
			Node() :
				firstItem(nullptr)
			{
				childs[0] = nullptr;
				childs[1] = nullptr;
				childs[2] = nullptr;
				childs[3] = nullptr;
			}

			ItemBounds bounds;			///< The bounds of this node
			Node* childs[4];			///< References to the sub-nodes, if they exist
			WrappedItem* firstItem;		///< Reference to the first item which is attached to this node
		};

		/**
		*  @brief
		*    Custom allocator type for Node allocation to reduce memory fragmentation/cache pollution
		*/
		typedef PagedAllocator<Node> NodeAllocator;

		/**
		*  @brief
		*    Custom allocator type for WrappedItem allocation to reduce memory fragmentation/cache pollution
		*/
		typedef PagedAllocator<WrappedItem> WrappedItemAllocator;

		/**
		*  @brief
		*    Custom map type which can quickly look up item data based on the item identity
		*/
		typedef boost::container::flat_map<ItemIdentity, WrappedItem*> ItemWrapMap;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    frees a node
		*/
		void deallocNode(Node* node)
		{
			mNodeAlloc.free(node);
		}

		/**
		*  @brief
		*    Allocates a new node
		*/
		Node* allocNode()
		{
			return mNodeAlloc.create();
		}

		/**
		*  @brief
		*    Allocates a new item wrapper
		*/
		WrappedItem* allocWrappedItem()
		{
			return mWrappedItemAlloc.alloc();
		}

		/**
		*  @brief
		*    Deallocates a item wrapper
		*/
		void deallocWrappedItem(WrappedItem* wrappedItem)
		{
			mWrappedItemAlloc.free(wrappedItem);
		}

		/**
		*  @brief
		*    cleans up a node instance, effectively deallocating the whole subtree formed by the node
		*/
		void clearNode(Node& node)
		{
			for (uint32 n = 0; n < 4; ++n)
			{
				if (node.childs[n] != nullptr)
				{
					clearNode(*node.childs[n]);
					deallocNode(node.childs[n]);
				}
			}
		}

		/**
		*  @brief
		*    Inserts the item into the subtree given by "node", potentially
		*    increasing subtree depth as long as MAX_DEPTH is not reached
		*/
		bool insertIntoTree(WrappedItem* const item, Node& node, uint32 currentDepth)
		{
			if (!areBoundsEncompassingBounds(node.bounds, item->bounds))
			{
				return false;
			}

			// When the item can be inserted completely into a child, do it
			if (currentDepth < mMaxDepth)
			{
				// When a single child is missing, we
				// haven't initialized the childs for this node
				// yet, so lets allocate all childs at once for
				// efficiency:
				if (node.childs[0] == nullptr)
				{
					for (uint32 childIndex = 0; childIndex < 4; childIndex++)
					{
						node.childs[childIndex] = allocNode();
						splitBoundsQuadrant(node.bounds, childIndex, node.childs[childIndex]->bounds);
					}
				}

				// Try to insert the child
				for (uint32 n = 0; n < 4; ++n)
				{
					const bool wasInserted = insertIntoTree(item, *node.childs[n], currentDepth + 1);
					if (wasInserted)
					{
						return true;
					}
				}
			}

			addItemToNodeContainment(item, node);
			return true;
		}

		/**
		*  @brief
		*    Takes the given item and inserts it into the whole tree, starting with the root node. When the
		*    item is outside of the root bounds, it will be assigned to the root node
		*/
		void linkItemIntoTree(WrappedItem* wrappedItem)
		{
			// TODO(oi) Automatic top-level tree creation and depth shift-down. For now, all "out-of-range" items will be put into another partition
			if (!insertIntoTree(wrappedItem, mRoot, 0))
			{
				mOutOfWorldItems.add(wrappedItem->item);
			}
		}

		/**
		*  @brief
		*    Unlinks, i.e. removes, the item from the tree. Does no deallocation!
		*/
		void unlinkItemFromTree(WrappedItem* wrappedItem)
		{
			// Unlink from the node's contained items list
			if (wrappedItem->containedNode)
			{
				// Fancy pointerpointer arithmetic to simplify the removal algorithm, stores
				// a reference to the "pointer to next element" of the predecessor of the "element to remove" so
				// we can directly re-validate it, ignorant of where it actually came from (either Node or WrappedItem)
				WrappedItem** predecessor = &wrappedItem->containedNode->firstItem;

				while (*predecessor != wrappedItem)
				{
					predecessor = &(*predecessor)->next;
				}

				// re-validate predecessor
				*predecessor = wrappedItem->next;

				// Invalidate the contained node
				wrappedItem->containedNode = nullptr;
			}
			else
			{
				mOutOfWorldItems.remove(this->getItemIdentity(wrappedItem->item));
			}
		}

		/**
		*  @brief
		*    Adds the given item to the list of items attached to the given node (i.e. putting it into the list of items for that node)
		*/
		static void addItemToNodeContainment(WrappedItem* const item, Node& node)
		{
			item->containedNode = &node;
			item->next = node.firstItem;
			node.firstItem = item;
		}

		/**
		*  @brief
		*    traverses the subtree given by the node and add all items which are in the given circle to the given item set
		*/
		template<class AnyBounds>
		static void traverseNode(Node& node, const AnyBounds& lookupBounds, ItemSet& outFoundObjects)
		{
			// Traverse sub nodes recursively when they exist
			if (node.childs[0] != nullptr)
			{
				for (uint32 n = 0; n < 4; ++n)
				{
					if (SpatialPartition2D<ItemType, SpatialItemTraits>::doBoundsOverlap(node.childs[n]->bounds, lookupBounds))
					{
						traverseNode(*node.childs[n], lookupBounds, outFoundObjects);
					}
				}
			}

			// Then look through all attached items to check if they really intersect the circle
			for (WrappedItem const* wrappedItem=node.firstItem; wrappedItem != nullptr; wrappedItem = wrappedItem->next)
			{
				const Item& item = wrappedItem->item;

				const ItemBounds& itemBounds = wrappedItem->bounds;

				if (SpatialPartition2D<ItemType, SpatialItemTraits>::doBoundsOverlap(itemBounds, lookupBounds))
				{
					outFoundObjects.insert(item);
				}
			}
		}

		static inline bool areBoundsEncompassingBounds(const ItemBounds& outerBounds, const ItemBounds& innerBounds) { return ItemTraits::areBoundsEncompassingBounds(outerBounds, innerBounds); }
		static inline void splitBoundsQuadrant(const ItemBounds& bounds, uint32 quadrant, ItemBounds& outQuadrantBounds) { return ItemTraits::splitBoundsQuadrant(bounds, quadrant, outQuadrantBounds); }


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef SpatialPartition2DBruteForceLookup<ItemType, ItemTraits> BruteForceLookup;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		NodeAllocator mNodeAlloc;				///< Allocator for quadtree nodes
		WrappedItemAllocator mWrappedItemAlloc;	///< Allocator for quadtree attached items
		Node mRoot;								///< Root node of the quadtree
		ItemWrapMap mWrappedItems;				///< Map of all items registered in the tree
		uint32 mMaxDepth;						///< max depth of the quadtree. Keep in mind memory usage for nodes is SUM(k, 0, MAX_DEPTH-1) { 4^k }
		BruteForceLookup mOutOfWorldItems;		///< special handling for objects which are out-of-bounds for the root node


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
