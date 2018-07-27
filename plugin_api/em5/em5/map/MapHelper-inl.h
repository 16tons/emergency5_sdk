// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MapHelper::MapHelper(qsf::Map& map) :
		qsf::MapHelper(map)
	{
		// Nothing here
	}

	inline MapHelper::~MapHelper()
	{
		// Nothing here
	}

	template <typename T>
	void MapHelper::showComponentofEntityIds(std::vector<uint64>& entityIds, bool show) const
	{
		for (uint64 entityId : entityIds)
		{
			// Get the entity
			const qsf::Entity* entity = getMap().getEntityById(entityId);
			if (nullptr != entity)
			{
				// Update the component
				T* component = entity->getComponent<T>();
				if (nullptr != component)
				{
					component->setActive(show);
				}
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
