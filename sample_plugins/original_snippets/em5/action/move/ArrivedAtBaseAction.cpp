// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/move/ArrivedAtBaseAction.h"
#include "em5/game/Game.h"
#include "em5/game/achievement/Achievements.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/plugin/Messages.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/timer/GameTimerManager.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ArrivedAtBaseAction::ACTION_ID = "em5::ArrivedAtBaseAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ArrivedAtBaseAction::ArrivedAtBaseAction() :
		Action(ACTION_ID)
	{
		// Nothing here
	}

	ArrivedAtBaseAction::~ArrivedAtBaseAction()
	{
		// Nothing here
	}

	void ArrivedAtBaseAction::init(const qsf::logic::TargetPoint& targetPoint)
	{
		mTargetPoint = targetPoint;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ArrivedAtBaseAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetPoint);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result ArrivedAtBaseAction::updateAction(const qsf::Clock&)
	{
		// Range check if entity is arrived at his target point
		glm::vec3 callerPosition = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>().getPosition();
		callerPosition.y = mTargetPoint.mPosition.y;
		if (qsf::game::DistanceHelper::getDistance(mTargetPoint.mPosition, callerPosition) > mTargetPoint.mTolerance)
		{
			// Additional check, if the entity has not reached the goal but is outside the map boundaries despawn it too, otherwise it will stay there forever because the player can't command it anymore
			if (EntityHelper(getEntity()).isInMapBoundaries())
			{
				// Error, entity can not reach its goal
				return qsf::action::RESULT_DONE;
			}
		}

		// Get the vehicle component
		VehicleComponent* vehicleComponent = getEntity().getComponent<VehicleComponent>();
		if (nullptr != vehicleComponent)
		{
			// Go recursive over all childs
			sendMessageForChilds(*vehicleComponent);
		}

		// Emit message
		qsf::MessageParameters parameters;
		parameters.setParameter("EntityId", getEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_TRANSPORTEDTOHQ, getEntityId()), parameters);

		// Fade out entity and delete it
		getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);

		// Install a second assassin to make sure the entity gets destroyed
		EM5_GAMETIMERS.addTimer(qsf::MessageConfiguration("em5::Game::destroyObject", getEntityId()), qsf::Time::fromSeconds(1.2f));

		return qsf::action::RESULT_CLEARLIST;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ArrivedAtBaseAction::sendMessageForChilds(VehicleComponent& vehicleComponent)
	{
		// Go over all entities in the container
		const qsf::Entity& unitEntity = getEntity();
		for (auto iterator : vehicleComponent.getLinks())
		{
			const qsf::ContainerLink& containerLink = *iterator.second;
			qsf::Entity& containedEntity = containerLink.getTargetEntity();

			// Call recursive for childs (to support the motorboat transporter, carrying an motorboat)
			VehicleComponent* childVehicleComponent = containedEntity.getComponent<VehicleComponent>();
			if (nullptr != childVehicleComponent)
			{
				sendMessageForChilds(*childVehicleComponent);
			}

			// Register the player as "has interacted with" event object
			EventIdComponent::registerPlayer(containedEntity, unitEntity);

			// Emit message
			qsf::MessageParameters parameters;
			parameters.setParameter("EntityId", containedEntity.getId());
			QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_TRANSPORTEDTOHQ, containedEntity.getId()), parameters);

			EntityHelper containedEntityHelper(containedEntity);
			if (containedEntityHelper.isPersonInjured())
			{
				// Emit message that person got treated by doctor
				//  -> This was introduced to fix possible gameplay blocker if players somehow manage to transport an injured person to HQ without him being treated by doctor before
				//  -> See EM_2017 bug 2567 "EMERGENCY 2017 | Emergency-Forum: "Patienten müssen von Notarzt versorgt werden 11/12, Patienten abtransportiert 12/12""
				QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_HEALTH_TREATED_PERSON, containedEntity.getId()));

				// Achievement for transported injured persons
				EM5_ACHIEVEMENT.addProgress(Achievements::EM5_ACHIEVEMENT_03, 1.0f);
			}
			else if (containedEntityHelper.isArrested())
			{
				// Achievement for transported arrested persons
				EM5_ACHIEVEMENT.addProgress(Achievements::EM5_ACHIEVEMENT_04, 1.0f);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
