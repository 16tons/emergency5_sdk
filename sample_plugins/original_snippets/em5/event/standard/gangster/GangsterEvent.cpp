// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/gangster/GangsterEvent.h"
#include "em5/component/spawnpoint/SpawnPointComponent.h"
#include "em5/logic/local/gangsters/GangsterBaseLogic.h"

#include <qsf/logic/gamelogic/GameLogicComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local helper functions                                ]
	//[-------------------------------------------------------]
	namespace detail
	{
		inline qsf::Entity* getOrSpawnEntity(qsf::Entity& entity)
		{
			// Given entity is possibly a spawn point
			SpawnPointComponent* spawnPointComponent = entity.getComponent<SpawnPointComponent>();
			if (nullptr != spawnPointComponent)
			{
				return spawnPointComponent->spawnRandom();
			}
			else
			{
				return &entity;
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	uint64 GangsterEvent::getGangsterId() const
	{
		return mGangster.valid() ? mGangster->getId() : qsf::getUninitialized<uint64>();
	}

	void GangsterEvent::setGangster(qsf::Entity& entity, bool resolveSpawnPoint)
	{
		if (resolveSpawnPoint)
		{
			mGangster = detail::getOrSpawnEntity(entity);
		}
		else
		{
			mGangster = entity;
		}
	}

	uint64 GangsterEvent::getVictimId() const
	{
		return mVictim.valid() ? mVictim->getId() : qsf::getUninitialized<uint64>();
	}

	void GangsterEvent::setVictim(qsf::Entity& entity)
	{
		mVictim = detail::getOrSpawnEntity(entity);
	}

	GangsterBaseLogic* GangsterEvent::getGangsterLogic() const
	{
		return mGangster.valid() ? mGangster->getOrCreateComponentSafe<qsf::GameLogicComponent>().getGameLogic<GangsterBaseLogic>() : nullptr;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	GangsterEvent::GangsterEvent()
	{
		// Nothing here
	}

	GangsterEvent::~GangsterEvent()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
