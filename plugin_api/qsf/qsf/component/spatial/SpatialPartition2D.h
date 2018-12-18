// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <glm/glm.hpp>

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>


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
	*    Generic base class for any implementation of a spatial partition for any generic item
	*
	*  @note
	*    This class is using policy-based design. A policy type "ItemTraits" is needed which has to
	*    fulfill the following contract:
	*
	*       typename ItemTraits::Identity											- A type which resembles the "id" of any item, e.g. uint32 when using indices/ids or event a pointer type
	*       typename ItemTraits::Bounds												- A type which resembles the "bounds" of any item, e.g. 2D AABB or a circle
	*       static ItemTraits::Bounds getBounds(Item i)								- A static method which fetches the 2D bounds of any element
	*       static bool doBoundsOverlap(ItemTraits::Bounds a, ItemTraits::Bounds b)	- A static method which tests for the overlap of two bounds
	*       static ItemTraits::Identity getIdentity(Item i)							- A static method which fetches the id of any element
	*
	*/
	//template<class ItemType, class ItemTraits = SpatialPartition2DItemTraits<ItemType>>
	template<class ItemType, class SpatialItemTraits>
	class SpatialPartition2D
	{
	public:
		typedef ItemType Item;									///< type for items inside the partition
		typedef typename SpatialItemTraits::Identity ItemIdentity;				///< type resembling an item id
		typedef typename SpatialItemTraits::Bounds ItemBounds;					///< type resembling item bounds
		typedef boost::container::flat_set<Item> ItemSet;		///< type resembling a set of items
		typedef SpatialItemTraits ItemTraits;							///< the item traits policy class

		virtual ~SpatialPartition2D() {}

		/**
		*  @brief
		*    Add a new item to the partition
		*
		*  @return
		*    Will return false if the item could not be added (i.e. identity conflict)
		*/
		virtual bool add(const Item& item) = 0;

		/**
		*  @brief
		*    Remove a item from the partition
		*
		*  @return
		*    Will return false if there was no such item to remove
		*/
		virtual bool remove(const ItemIdentity& id) = 0;

		/**
		*  @brief
		*    In case the item moved (the getPosition() policy now returns another position) this will
		*    update the item inside the partition so that it will be found at its new position.
		*
		*  @note
		*    Failure to call this method when the item moves will lead to wrong results when doing lookups!
		*/
		virtual void update(const ItemIdentity& id) = 0;

		/**
		*  @brief
		*    Looks up all elements which are located inside the given circle and puts them into the output set
		*/
		virtual void lookUpElementsInRange(const ItemBounds& bounds, ItemSet& outFoundObjects) = 0;

	protected:
		// Helper methods to interface with the ItemTraits contract
		static inline ItemBounds getItemBounds(const Item& item) { return ItemTraits::getBounds(item); }
		static inline ItemIdentity getItemIdentity(const Item& item) { return ItemTraits::getIdentity(item); }
		template<class AnyBoundsA, class AnyBoundsB>
		static inline bool doBoundsOverlap(const AnyBoundsA& a, const AnyBoundsB& b) { return ItemTraits::doBoundsOverlap(a, b); }
	};

	/**
	*  @brief
	*    Small intermediate helper class which does item-management on a list, but itself does
	*    not provide lookup functionality
	*/
	template<class ItemType, class SpatialItemTraits>
	class SpatialPartition2DWithManagedList : public SpatialPartition2D<ItemType, SpatialItemTraits>
	{
	public:
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::Item Item;
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::ItemIdentity ItemIdentity;
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::ItemBounds ItemBounds;
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::ItemSet ItemSet;
		typedef typename SpatialPartition2D<ItemType, SpatialItemTraits>::ItemTraits ItemTraits;

		virtual bool add(const Item& item) override
		{
			return mItems.emplace(this->getItemIdentity(item), item).second;
		}

		virtual bool remove(const ItemIdentity& id)
		{
			typename ItemMap::iterator found = mItems.find(id);

			if (found != mItems.end())
			{
				mItems.erase(found);
				return true;
			}
			return false;
		}

		virtual void update(const ItemIdentity& id)
		{
			// We have no structures that would benefit from modification here!
		}

	protected:
		typedef boost::container::flat_map<ItemIdentity, Item> ItemMap;
		ItemMap mItems;
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
