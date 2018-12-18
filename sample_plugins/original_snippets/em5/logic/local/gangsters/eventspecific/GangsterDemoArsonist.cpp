// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/gangsters/eventspecific/GangsterDemoArsonist.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/SignalAction.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/move/TurnToAction.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf/logic/action/WaitAction.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Locally used definitions                              ]
	//[-------------------------------------------------------]
	namespace
	{
		const qsf::StringHash MESSAGE_START_FIRE = "em5::GangsterDemoArsonist_start_fire";
	}


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterDemoArsonist::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterDemoArsonist");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterDemoArsonist::GangsterDemoArsonist() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mWaitTimeBeforeEscape(qsf::Time::fromSeconds(5.0f)),
		mState(STATE_INIT)
	{
		mSetGangsterFlagOnStartup = false;
	}

	void GangsterDemoArsonist::setWaitTimeBeforeEscape(qsf::Time time)
	{
		mWaitTimeBeforeEscape = time;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool GangsterDemoArsonist::onStartup()
	{
		if (!GangsterBaseLogic::onStartup())
			return false;

		// Make sure he is immune to fire
		getEntitySafe().getOrCreateComponentSafe<FireReceiverComponent>().setActive(false);

		mStartFireMessageProxy.registerAt(qsf::MessageConfiguration(MESSAGE_START_FIRE, getEntityId()), boost::bind(&GangsterDemoArsonist::onStartFireMessage, this, _1));

		return true;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterDemoArsonist::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		// Nothing special to do in here, all gangster behavior is handled in "onIdle"
	}

	void GangsterDemoArsonist::onIdle()
	{
		// Nothing here
	}

	void GangsterDemoArsonist::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		// Nothing here
	}

	void GangsterDemoArsonist::onNoUnitsSpotted()
	{
		// Nothing here
	}

	void GangsterDemoArsonist::onWaypointReached(const Waypoint& waypoint)
	{
		// Set fire on building
		BuildingComponent* buildingComponent = waypoint.mEntity->getComponent<BuildingComponent>();
		if (nullptr != buildingComponent)
		{
			qsf::ActionComponent& actionComponent = getEntitySafe().getOrCreateComponentSafe<qsf::ActionComponent>();

			if (!buildingComponent->isBurning())
			{
				// Push actions
				actionComponent.clearPlan();
				actionComponent.pushAction<TurnToAction>(action::COMMAND_HIGH).init(*waypoint.mEntity);
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH).init("em5/skeleton/person/female_manipulate_object_kneeling_01_start", true, false, false, qsf::Time::fromSeconds(0.3f));
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH).init("em5/skeleton/person/female_manipulate_object_kneeling_02_loop", true, true, false, qsf::Time::ZERO, qsf::Time::fromSeconds(3.0f));
				actionComponent.pushAction<SignalAction>(action::COMMAND_HIGH).init(qsf::MessageConfiguration(MESSAGE_START_FIRE, getEntityId(), waypoint.mEntity->getId()));
				actionComponent.pushAction<PlayAnimationAction>(action::COMMAND_HIGH).init("em5/skeleton/person/female_manipulate_object_kneeling_02_end", true);
			}

			// Run to exit if all is done
			if (mWaypoints.empty())
			{
				// Wait for X seconds
				actionComponent.pushAction<qsf::WaitAction>().init(mWaitTimeBeforeEscape);

				escape(ESCAPE_FOOT_SHORT);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void GangsterDemoArsonist::onStartFireMessage(const qsf::MessageParameters& parameters)
	{
		const uint64 buildingEntityId = parameters.getFilter(2);

		FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();
		EntityHelper(buildingEntityId, getMap()).startFire(freeplayEvent);

		// Mark him as gangster now
		setAsGangsterFlagged(true);

		// TODO(fw): Mark the other one as gangster as well

	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
