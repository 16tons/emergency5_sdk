// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/firefighters/MotorboatTransporterLogic.h"
#include "em5/component/vehicle/BoatComponent.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/physics/collision/CollisionComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 MotorboatTransporterLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::MotorboatTransporterLogic");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	MotorboatTransporterLogic::MotorboatTransporterLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mShowBoat(false)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool MotorboatTransporterLogic::onStartup()
	{
		// Backup the position of the motorboat (optimal position is edited)
		RoadVehicleComponent* roadVehicleComponent = getEntity()->getComponent<RoadVehicleComponent>();
		if (nullptr != roadVehicleComponent)
		{
			qsf::Entity* boatEntity = getMap().getEntityById(roadVehicleComponent->getSpecialEntity());
			if (boatEntity)
			{
				mSpawnBoatRegistration.registerAt(qsf::MessageConfiguration(Messages::EM5_SPAWN_UNIT, boatEntity->getId()), boost::bind(&MotorboatTransporterLogic::onSpawnBoatRegistration, this, _1));
			}
		}

		// Unhide and maybe link back to the crane entity (can be saved at startup())
		mSpawnRegistration.registerAt(qsf::MessageConfiguration(Messages::EM5_SPAWN_UNIT, getEntityId()), boost::bind(&MotorboatTransporterLogic::onSpawnRegistration, this, _1));

		return true;
	}

	void MotorboatTransporterLogic::onShutdown()
	{
		// Unregister messages
		mSpawnRegistration.unregister();
		mSpawnBoatRegistration.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void MotorboatTransporterLogic::onSpawnRegistration(const qsf::MessageParameters& parameters)
	{
		// Called after the unit is ordered and linked to the parent

		RoadVehicleComponent* roadVehicleComponent = getEntity()->getComponent<RoadVehicleComponent>();
		if (nullptr == roadVehicleComponent)
			return;

		qsf::Entity* boatEntity = getMap().getEntityById(roadVehicleComponent->getSpecialEntity());
		if (nullptr == boatEntity)
			return;

		if (mShowBoat)
		{
			qsf::game::HiddenComponent::setHidden(*boatEntity, false);
		}
		else
		{
			// Destroy the optical boat entity
			getEntity()->getMap().destroyEntityById(boatEntity->getId());

			// Lower the crane
			VehicleCraneComponent* vehicleCraneComponent = getEntity()->getComponent<VehicleCraneComponent>();
			if (nullptr != vehicleCraneComponent)
			{
				vehicleCraneComponent->stopAllAnimation();
			}
		}

		// Remove collision box of boat
		qsf::CollisionComponent* collisionComponent = boatEntity->getComponent<qsf::CollisionComponent>();
		if (nullptr != collisionComponent)
			collisionComponent->setActive(false);

		// Only once
		mSpawnRegistration.unregister();

		// We don't need the information of spawn boat
		mSpawnBoatRegistration.unregister();
	}

	void MotorboatTransporterLogic::onSpawnBoatRegistration(const qsf::MessageParameters& parameters)
	{
		// Called only if the boat is ordered
		mShowBoat = true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
