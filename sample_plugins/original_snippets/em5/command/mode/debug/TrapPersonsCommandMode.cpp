// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/TrapPersonsCommandMode.h"
#include "em5/command/CommandContext.h"
#include "em5/map/EntityHelper.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/freeplay/event/FreeplayEventManager.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/freeplay/FreeplaySystem.h"
#include "em5/EM5Helper.h"

#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/map/Entity.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TrapPersonsCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::TrapPersonsCommandMode");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TrapPersonsCommandMode::TrapPersonsCommandMode() :
		DebugCommandMode(PLUGINABLE_ID),
		mSpawnGangster(false),
		mSpawnHostage(false)
	{
		// Nothing here
	}

	TrapPersonsCommandMode::~TrapPersonsCommandMode()
	{
		// Nothing here
	}

	void TrapPersonsCommandMode::spawnGangster(bool spawnGangster, bool spawnHostage)
	{
		mSpawnGangster = spawnGangster;
		mSpawnHostage = spawnHostage;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void TrapPersonsCommandMode::startup()
	{
		// Nothing here
	}

	void TrapPersonsCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool TrapPersonsCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		// Is there a target entity selected at all?
		if (nullptr != commandContext.mTargetEntity)
		{
			qsf::Entity* targetEntity = commandContext.mTargetEntity;
			const EntityHelper entityHelper(*targetEntity);

			BuildingComponent* buildingComponent = targetEntity->getComponent<BuildingComponent>();
			if (nullptr != buildingComponent)
			{
				// Add the building to the event, so we have the right logic
				if (entityHelper.getFreeplayEvent() == nullptr)
				{
					FreeplayEvent* freeplayEvent = EM5_FREEPLAY.getEventManager().getLastEvent();
					EventSpreadHelper::setEventIdComponent(freeplayEvent, *targetEntity, eventspreadreason::NO_REASON);
				}

				if (mSpawnGangster)
				{
					int numberHostage = (mSpawnHostage ? 2 : 0);
					buildingComponent->spawnGangsterInside(2, numberHostage, 0);
				}
				else
				{
					if (buildingComponent->isContainingTrappedPersons())
					{
						buildingComponent->untrapPersons();
					}
					else
					{
						buildingComponent->setupTrappedPersons(2, 2);

						// TODO(mk) For testing change this value here
						buildingComponent->setIsDoorBlocked(false);

						// Start a new fire
						entityHelper.startFire(getCurrentFreeplayEvent());
					}
				}
			}
			else
			{
				VehicleComponent* vehicleComponent = targetEntity->getComponent<VehicleComponent>();
				if (nullptr != vehicleComponent)
				{
					vehicleComponent->trapPersons();
					vehicleComponent->setDamageState(DamageComponent::DAMAGESTATE_DAMAGE_LEVEL2);
				}
			}
		}

		// Always return false to keep the command mode alive
		return false;
	}

	void TrapPersonsCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		std::string displayedText = "Trap persons";
		if (mSpawnHostage)
			displayedText = "Spawn gangster with hostage in building";
		else
			displayedText = "Spawn gangster in building";

		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(displayedText, getMousePosition() + glm::vec2(16, 8)));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
