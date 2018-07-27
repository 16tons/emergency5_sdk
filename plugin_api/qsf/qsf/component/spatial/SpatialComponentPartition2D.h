// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/Component.h"
#include "qsf/component/spatial/SpatialPartition2DQuadtree.h"
#include "qsf/base/manager/FastPodAllocator.h"

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class SpatialPartitionMemberComponent;
}


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
	*    Specialized implementation of a 2D partition for components. Relies on a specific implementation
	*    of SpatialPartition2D. To provide a implementation, create a subclass which provides the corresponding
	*    SpatialPartition2D in the constructor.
	*/
	class SpatialComponentPartition2D : public boost::noncopyable
	{
	public:
		/**
		*  @brief
		*    Data structure which is used for each item which gets registered in the spatial partition.
		*    Also servers as the implementation of the SpatialPartition2D::ItemTraits policy
		*    @see SpatialPartition2D
		*/
		struct ComponentItem : public FastPodAllocator<ComponentItem>
		{
			typedef uint64 Identity; ///< ID type for every item
			struct Bounds
			{
				glm::vec2 min;
				glm::vec2 max;
			};

			struct CircleBounds
			{
				glm::vec2 center;
				float radius;
			};

			uint64							 entityId;		///< Cached entity ID for this item
			Component*						 component;		///< Stored component in this item
			SpatialPartitionMemberComponent* cachedMember;	///< Cached spatial partition member component for this item for quick position and bounds lookup

			/**
			*  @brief
			*    Implementation of the SpatialPartition2D::ItemTraits::getPosition contract
			*  @note
			*    Could inline this if SpatialPartitionMemberComponent headers would be included, but first check if it is worth it.
			*/
			static Bounds getBounds(const ComponentItem& item);

			static bool doBoundsOverlap(const Bounds& a, const Bounds& b)
			{
				// TODO(fw): Code looks very similar to AmbientSpotAudioSourceItem::doBoundsOverlap - please refactore

				// Do a SAT (separating axis test) for the two aligned axes
				for (uint32 n = 0; n < 2; ++n)
				{
					if (a.max[n] < b.min[n] || a.min[n] > b.max[n])
					{
						return false;
					}
				}

				return true;
			}

			static bool doBoundsOverlap(const Bounds& a, const CircleBounds& b)
			{
				// TODO(fw): Code looks very similar to AmbientSpotAudioSourceItem::doBoundsOverlap - please refactore

				float sqDist = 0.0f;

				const glm::vec2& min = a.min;
				const glm::vec2& max = a.max;

				for (uint32 n = 0; n < 2; ++n)
				{
					float v = b.center[n];

					if (v < min[n])
					{
						sqDist += (min[n] - v) * (min[n] - v);
					}

					if (v > max[n])
					{
						sqDist += (v - max[n]) * (v - max[n]);
					}
				}

				return sqDist < (b.radius * b.radius);
			}

			static void splitBoundsQuadrant(const Bounds& bounds, uint32 quadrant, Bounds& outQuadrantBounds)
			{
				const glm::vec2 halfSize = (bounds.max - bounds.min) * 0.5f;

				glm::vec2 quadrantMin = (bounds.max + bounds.min) * 0.5f;

				quadrantMin.x += (quadrant & 1) ? 0.0f : -halfSize.x;
				quadrantMin.y += (quadrant & 2) ? 0.0f : -halfSize.y;

				outQuadrantBounds.min = quadrantMin;
				outQuadrantBounds.max = outQuadrantBounds.min + halfSize;
			}

			static bool areBoundsEncompassingBounds(const Bounds& outerBounds, const Bounds& innerBounds)
			{
				return
					outerBounds.max.x > innerBounds.max.x &&
					outerBounds.max.y > innerBounds.max.y &&

					outerBounds.min.x < innerBounds.min.x &&
					outerBounds.min.y < innerBounds.min.y;
			}

			/**
			*  @brief
			*    Implementation of the SpatialPartition2D::ItemTraits::getIdentity contract
			*/
			static inline const Identity& getIdentity(const ComponentItem& item)
			{
				return item.entityId;
			}

			inline bool operator == (const ComponentItem& other) const
			{
				return entityId == other.entityId;
			}

			inline bool operator != (const ComponentItem& other) const
			{
				return entityId != other.entityId;
			}

			inline bool operator < (const ComponentItem& other) const
			{
				return entityId < other.entityId;
			}
		};

		/**
		*  @brief
		*    Type of the spatial partition interface which is used to speed up range queries
		*/
		typedef SpatialPartition2DQuadtree<ComponentItem, ComponentItem> PartitionImplementation;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		~SpatialComponentPartition2D();

		void addComponent(Component* component);
		void removeByEntityId(uint64 entityId);
		void updateByEntityId(uint64 entityId);

		void findInBounds(const glm::vec2& min, const glm::vec2& max, std::vector<Component*>& outComponents) const;
		void findInCircle(const glm::vec2& center, float radius, std::vector<Component*>& outComponents) const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Initializes the component partition. It will use the given implementation. Memory ownership goes to
		*    the SpatialComponentPartition2D instance which will delete the imlementation with stock C++ "delete"
		*/
		SpatialComponentPartition2D(PartitionImplementation* usePartition);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Pointer to a dynamically managed partition implementation. This is no reference to highlight the
		*    fact that this is actually managed by the implementation and it is not relying on external memory
		*/
		PartitionImplementation* mPartition;
	};


	template<class PartitionType>
	class SpatialComponentPartition2DSpecialized : public SpatialComponentPartition2D
	{
	public:
		SpatialComponentPartition2DSpecialized(SpatialComponentPartition2D::ComponentItem::Bounds worldBounds) : SpatialComponentPartition2D(new PartitionType(worldBounds)) {}
	};

	typedef SpatialComponentPartition2DSpecialized<SpatialPartition2DQuadtree<SpatialComponentPartition2D::ComponentItem, SpatialComponentPartition2D::ComponentItem>> SpatialComponentPartition2DQuadtree;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
