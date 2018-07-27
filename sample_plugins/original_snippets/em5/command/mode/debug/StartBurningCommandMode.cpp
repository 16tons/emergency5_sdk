// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/StartBurningCommandMode.h"
#include "em5/command/CommandContext.h"
#include "em5/map/EntityHelper.h"
#include "em5/fire/component/FireComponent.h"
#include "em5/fire/component/ComplexFireComponent.h"
#include "em5/freeplay/FreeplaySystem.h"
#include "em5/freeplay/event/FreeplayEventManager.h"
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
	const uint32 StartBurningCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::StartBurningCommandMode");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	bool StartBurningCommandMode::execute(qsf::game::CommandContext& commandContext)
	{
		// Is there a target entity selected at all?
		if (nullptr != commandContext.mTargetEntity)
		{
			qsf::Entity* targetEntity = commandContext.mTargetEntity;
			const EntityHelper entityHelper(*targetEntity);

			// Either incinerate, damage or extinguish the entity
			if (entityHelper.isBurned())
			{
				// Remove the fire
				entityHelper.cleanupFire();
			}
			else if (entityHelper.isBurning())
			{
				// Do fire damage to the entity
				FireComponent* fireComponent = targetEntity->getComponent<FireComponent>();
				if (nullptr == fireComponent)
				{
					const ComplexFireComponent* complexFireComponent = targetEntity->getComponent<ComplexFireComponent>();
					if (nullptr != complexFireComponent)
					{
						fireComponent = complexFireComponent->getBurningFireComponent();
					}
				}

				// Do 10% damage per click
				if (nullptr != fireComponent)
				{
					const float amountFireDamage = fireComponent->getMaximumLife() / 10.0f;
					fireComponent->forceFireDamage(amountFireDamage);
				}
			}
			else
			{
				// Start a new fire
				entityHelper.startFire(EM5_FREEPLAY.getEventManager().getLastEvent());
			}
		}

		// Always return false to keep the command mode alive
		return false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StartBurningCommandMode::StartBurningCommandMode() :
		DebugCommandMode(PLUGINABLE_ID)
	{
		// Nothing here
	}

	StartBurningCommandMode::~StartBurningCommandMode()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void StartBurningCommandMode::startup()
	{
		// Nothing here
	}

	void StartBurningCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool StartBurningCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet&)
	{
		return execute(commandContext);
	}

	void StartBurningCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Debug output at the player's mouse cursor
		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest("Start Burning", getMousePosition() + glm::vec2(16, 8)));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
