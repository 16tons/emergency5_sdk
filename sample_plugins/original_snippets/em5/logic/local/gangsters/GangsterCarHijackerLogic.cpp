// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/ExitVehicleAction.h"
#include "em5/action/move/FullBrakeAction.h"
#include "em5/action/move/escape/EscapeFootShortAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/ai/MoverType.h"
#include "em5/logic/local/gangsters/GangsterCarHijackerLogic.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/EscapeSpecsGroup.h"
#include "em5/EM5Helper.h"

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/map/component/MapPropertiesComponent.h>
#include <qsf/map/Map.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterCarHijackerLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterCarHijackerLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterCarHijackerLogic::GangsterCarHijackerLogic() :
		GangsterBaseLogic(GAMELOGIC_TYPE_ID),
		mCarChaseTimer(qsf::Time::MAX)
	{
		mEquippedWeaponTypes.set(weapon::PISTOL);
		mEquippedWeaponTypes.set(weapon::BASEBALLBAT);
		mEquippedWeaponTypes.set(weapon::KNIFE);

		mAlwaysShowWeapon = false;

		// We want to receive "onUnitsSpotted" and "onNoUnitsSpotted" callbacks
		mUseOldSpottedUnitsLogic = true;

		mMessageProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_ACTION_GUNSHOT), boost::bind(&GangsterCarHijackerLogic::onShotAt, this, _1));
	}

	void GangsterCarHijackerLogic::startCarChase()
	{
		mCarChaseTimer = qsf::Time::fromSeconds(EscapeSpecsGroup::getInstanceSafe().getHelicopterFollowTime());
	}

	void GangsterCarHijackerLogic::abortCarChase()
	{
		mCarChaseTimer = qsf::Time::MAX;
	}

	void GangsterCarHijackerLogic::escapeOnFoot(qsf::Entity& vehicle)
	{
		// Helicopter is probably chasing the gangster!
		qsf::ActionComponent* gangsterActionComponent = getEntity()->getOrCreateComponent<qsf::ActionComponent>();
		qsf::ActionComponent* vehicleActionComponent = vehicle.getOrCreateComponent<qsf::ActionComponent>();
		if (gangsterActionComponent->getPlan().isEmpty()
			|| vehicleActionComponent->getPlan().isEmpty()
			|| vehicleActionComponent->getCurrentAction()->getTypeId() != ExitVehicleAction::ACTION_ID)
		{
			// Gangster is not currently exiting the vehicle he is riding and does not plan to do so. Make him exit the vehicle!
			gangsterActionComponent->clearPlan();

			// If the entity is moving, enqueue a FullBrakeAction to make it stop
			const qsf::MovableComponent* movableComponent = getEntity()->getComponent<qsf::MovableComponent>();
			if (nullptr != movableComponent && movableComponent->isMoving())
			{
				gangsterActionComponent->pushAction<FullBrakeAction>(action::BLOCKING);
			}

			{
				qsf::ai::NavigationComponent& navigationComponent = getEntity()->getOrCreateComponentSafe<qsf::ai::NavigationComponent>();
				navigationComponent.dissolveDriverVehicleRelation();
				navigationComponent.setMoverType(MoverType::GANGSTER_PERSON);
			}

			// Add the exit vehicle action
			vehicleActionComponent->pushAction<ExitVehicleAction>(action::BLOCKING);

			// Flee on foot
			escape(GangsterBaseLogic::ESCAPE_FOOT_SHORT);
		}
		mCarChaseTimer = qsf::Time::MAX;

		// TODO(fw): I'm not sure if my change here is correct, original is out-commented below
		//vehicle.getOrCreateComponent<VehicleComponent>()->setDamagePercent(0.01f);
		vehicle.getOrCreateComponent<VehicleComponent>()->setDamageState(DamageComponent::DAMAGESTATE_DAMAGE_LEVEL2);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool GangsterCarHijackerLogic::onStartup()
	{
		mNeedsUpdateInContainer = true;

		// Call base implementation
		return GangsterBaseLogic::onStartup();
	}

	void GangsterCarHijackerLogic::onShutdown()
	{
		mMessageProxy.unregister();

		// Call base implementation
		GangsterBaseLogic::onShutdown();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	void GangsterCarHijackerLogic::onSimulationUpdate(const qsf::JobArguments& jobArguments)
	{
		qsf::Entity* vehicle = EntityHelper(*getEntity()).getContainerEntity();
		if (nullptr == vehicle)
		{
			// Gangster has no ride, nothing to do here
			return;
		}

		updateCarChase(jobArguments.getTimePassed());

		if (EntityHelper(*vehicle).isRoadVehicleDamaged())
		{
			escapeOnFoot(*vehicle);
		}
	}

	void GangsterCarHijackerLogic::onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits)
	{
		if (sightedPoliceUnits.empty())
		{
			return;
		}

		if (nullptr == EntityHelper(*getEntity()).getContainerEntity())
		{
			// Gangster has no ride, nothing to do here
			return;
		}
	}

	void GangsterCarHijackerLogic::onShotAt(const qsf::MessageParameters& parameters)
	{
		qsf::Entity* vehicle = EntityHelper(*getEntity()).getContainerEntity();
		if (nullptr == vehicle)
		{
			// Gangster has no ride, nothing to do here
			return;
		}
		if (nullptr == vehicle->getComponent<VehicleComponent>())
		{
			// Gangster has no ride, nothing to do here
			return;
		}

		uint64 id = qsf::getUninitialized<uint64>();
		parameters.getParameter("ShooteeId", id);
		if (id != vehicle->getId())
			return;

		escapeOnFoot(*vehicle);
	}

	void GangsterCarHijackerLogic::updateCarChase(const qsf::Time& timePassed)
	{
		if (mCarChaseTimer >= qsf::Time::MAX || mCarChaseTimer <= qsf::Time::ZERO)
			return;

		qsf::Entity* carEntity = EntityHelper(*getEntity()).getContainerEntity();
		if (nullptr == carEntity)
			return;

		mCarChaseTimer -= timePassed;

		{ // Be fair to the player: It's possible that the event triggers close too the map border and the player
		  // has nearly no chance of winning the event, let the gangster escape on foot in this case
			const qsf::MapPropertiesComponent* mapPropertiesComponent = QSF_MAINMAP.getCoreEntity().getComponent<qsf::MapPropertiesComponent>();
			if (nullptr != mapPropertiesComponent)
			{
				const float TOLLERANCE = 50.0f;
				const glm::vec3& position = EntityHelper(*carEntity).getPosition();
				const glm::vec3 differenceToTopLeft = glm::abs(position - mapPropertiesComponent->getMapBoundaryTopLeft());
				const glm::vec3 differenceToBottomRight = glm::abs(position - mapPropertiesComponent->getMapBoundaryBottomRight());
				if (differenceToTopLeft.x < TOLLERANCE || differenceToTopLeft.z < TOLLERANCE ||
					differenceToBottomRight.x < TOLLERANCE || differenceToBottomRight.z < TOLLERANCE)
				{
					mCarChaseTimer = qsf::Time::ZERO;
				}
			}
		}

		if (mCarChaseTimer <= qsf::Time::ZERO)
		{
			escapeOnFoot(*carEntity);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
