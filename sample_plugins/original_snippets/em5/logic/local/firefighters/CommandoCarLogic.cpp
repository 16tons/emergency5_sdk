// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/firefighters/CommandoCarLogic.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/specs/FireSpecsGroup.h"
#include "em5/logic/HintHelper.h"
#include "em5/plugin/Jobs.h"

#include <qsf/job/JobArguments.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 CommandoCarLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::CommandoCarLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	CommandoCarLogic::CommandoCarLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mAreaExtinguishCooldownTimer(qsf::Time::ZERO)
	{
		// Nothing here
	}

	bool CommandoCarLogic::isAreaExtinguishSkillReady() const
	{
		return (mAreaExtinguishCooldownTimer <= qsf::Time::ZERO);
	}

	void CommandoCarLogic::activateAreaExtinguishSkill()
	{
		mAreaExtinguishCooldownTimer = qsf::Time::fromSeconds(FireSpecsGroup::getInstance()->getCooldownCommandoCarExtinguish());

		// Register job
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&CommandoCarLogic::updateSimulation, this, _1), jobConfiguration);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool CommandoCarLogic::onStartup()
	{
		return true;
	}

	void CommandoCarLogic::onShutdown()
	{
		mSimulationJobProxy.unregister();

		// Hide progressbar
		StatusOverlayComponent::hideBarOverlay(*getEntity(), StatusOverlayComponent::BAR_TYPE_PROGRESS);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void CommandoCarLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Update cooldown timer
		if (mAreaExtinguishCooldownTimer > qsf::Time::ZERO)
		{
			// Update progressbar
			float progress = mAreaExtinguishCooldownTimer.getSeconds() / FireSpecsGroup::getInstance()->getCooldownCommandoCarExtinguish();
			StatusOverlayComponent::showBarOverlay(*getEntity(), StatusOverlayComponent::BAR_TYPE_PROGRESS, progress);

			mAreaExtinguishCooldownTimer -= jobArguments.getTimePassed();
			if (mAreaExtinguishCooldownTimer <= qsf::Time::ZERO)
			{
				// Clamp value to zero
				mAreaExtinguishCooldownTimer = qsf::Time::ZERO;

				// Hide progressbar
				StatusOverlayComponent::hideBarOverlay(*getEntity(), StatusOverlayComponent::BAR_TYPE_PROGRESS);

				CommandableComponent* commandableComponent = getEntity()->getComponent<CommandableComponent>();
				if (nullptr != commandableComponent)
				{
					// Play hint
					HintHelper::showHint("ID_HINT_LF_READY_01", commandableComponent->getPlayerId());
				}

				// Job done
				mSimulationJobProxy.unregister();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
