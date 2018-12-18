// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/spatial/SpatialPartition2D.h"

#include <glm/glm.hpp>


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
	*    Brute force linear lookup implementation of a 2d spatial partition
	*
	*  @note
	*    Mostly used as a "it is ensured it behaves as wanted" reference implementation when implementing
	*    new partition algorithms. Don't use it in production if you are not perfectly sure that your algorithsm
	*    benefit from brute force lookup (which might be the case if your search space is small enough so the overhead
	*    of constructing specialized search structures is bigger than the actual lookup mathematics cost)
	*/
	template<class ItemType, class SpatialItemTraits>
	class SpatialPartition2DBruteForceLookup : public SpatialPartition2DWithManagedList<ItemType, SpatialItemTraits>
	{
	public:
		typedef typename SpatialPartition2DWithManagedList<ItemType, SpatialItemTraits>::Item Item;
		typedef typename SpatialPartition2DWithManagedList<ItemType, SpatialItemTraits>::ItemIdentity ItemIdentity;
		typedef typename SpatialPartition2DWithManagedList<ItemType, SpatialItemTraits>::ItemBounds ItemBounds;
		typedef typename SpatialPartition2DWithManagedList<ItemType, SpatialItemTraits>::ItemSet ItemSet;
		typedef typename SpatialPartition2DWithManagedList<ItemType, SpatialItemTraits>::ItemTraits ItemTraits;

		SpatialPartition2DBruteForceLookup() {}

		void lookUpElementsInRange(const ItemBounds& bounds, ItemSet& outFoundObjects) override
		{
			lookUpElementsInAnyRange(bounds, outFoundObjects);
		}

		template<typename AnyBounds>
		void lookUpElementsInAnyRange(const AnyBounds& bounds, ItemSet& outFoundObjects)
		{
			for (const typename ItemMap::value_type& itemPair : this->mItems)
			{
				if (this->doBoundsOverlap(this->getItemBounds(itemPair.second), bounds))
				{
					outFoundObjects.insert(itemPair.second);
				}
			}
		}

	protected:
		typedef typename SpatialPartition2DWithManagedList<ItemType, SpatialItemTraits>::ItemMap ItemMap;
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
