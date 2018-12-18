// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/base/PersonInBuildingLogic.h"
#include "em5/audio/AudioHelper.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/map/CollisionHelper.h"
#include "em5/plugin/Jobs.h"

#include <qsf/job/JobArguments.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PersonInBuildingLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::PersonInBuildingLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PersonInBuildingLogic::PersonInBuildingLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mNumberValidTeamUnitsNeeded(1),
		mRescuePersonTimerMax(qsf::Time::fromSeconds(1.0f)),
		mRescuePersonTimer(qsf::Time::ZERO),
		mIsJobActive(false)
	{
		// Nothing here
	}

	void PersonInBuildingLogic::clearAllowedUnits()
	{
		mAllowedUnits.clear();
	}

	void PersonInBuildingLogic::addAllowedUnit(const qsf::StringHash& unitType)
	{
		mAllowedUnits.emplace(unitType);
	}

	bool PersonInBuildingLogic::isTeamComplete() const
	{
		std::vector<qsf::Entity*> policeUnits = mBuildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_PROGRESS);
		int numberValidTeamUnits = 0;

		for (qsf::Entity* entity : policeUnits)
		{
			CommandableComponent* commandableComponent = entity->getComponent<CommandableComponent>();
			if (mAllowedUnits.count(commandableComponent->getUnitType()) > 0)
			{
				++numberValidTeamUnits;
			}
		}

		return (numberValidTeamUnits >= mNumberValidTeamUnitsNeeded);
	}

	bool PersonInBuildingLogic::isUnitAllowed(const qsf::Entity& entity)
	{
		CommandableComponent* commandableComponent = entity.getComponent<CommandableComponent>();
		if (nullptr == commandableComponent)
			return false;

		// Check if the unit type is registered as allowed unit
		return (mAllowedUnits.count(commandableComponent->getUnitType()) > 0);
	}

	void PersonInBuildingLogic::startFreeingCivil()
	{
		if (mIsJobActive)
			return;

		if (isBuildingEmpty())
		{
			// Building is empty
			return;
		}

		// Activate job by registering via the job proxy
		mUpdateJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&PersonInBuildingLogic::updateSimulation, this, _1));
		mRescuePersonTimer = mRescuePersonTimerMax;
		mIsJobActive = true;
	}

	bool PersonInBuildingLogic::isBuildingEmpty()
	{
		if (!mBuildingComponent.valid())
			return false;

		if (!mBuildingComponent->isContainingTrappedPersons() &&
				mBuildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_HOSTAGE).empty())
		{
			return true;
		}

		return false;
	}

	void PersonInBuildingLogic::playEvacuateAudio()
	{
		AudioHelper::playBuildingEvacuateSound2(mAudioProxy, getEntitySafe());
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool PersonInBuildingLogic::onStartup()
	{
		mBuildingComponent = getEntity()->getComponent<BuildingComponent>();

		// Call base implementation
		return qsf::GameLogic::onStartup();
	}

	void PersonInBuildingLogic::onShutdown()
	{
		mUpdateJobProxy.unregister();

		// Call base implementation
		qsf::GameLogic::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PersonInBuildingLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		mRescuePersonTimer -= jobArguments.getTimePassed();
		if (mRescuePersonTimer <= qsf::Time::ZERO)
		{
			mRescuePersonTimer = mRescuePersonTimerMax;

			// Free one person
			mBuildingComponent->untrapPerson_SingleInjured();

			if (isBuildingEmpty())
			{
				// Building is empty
				mUpdateJobProxy.unregister();
				mIsJobActive = false;
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
