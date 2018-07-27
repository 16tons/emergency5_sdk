// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/fire/DisconnectHydrantAction.h"
#include "em5/action/base/PlayAudioAction.h"
#include "em5/action/equipment/GetEquipmentAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/base/EquipmentAssets.h"
#include "em5/base/AudioAssets.h"
#include "em5/component/vehicle/RoadVehicleComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/component/objects/HistoricWellComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/fire/GetHoseHistoricalCommand.h"
#include "em5/command/police/historic/GetSwordCommand.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/fire/component/HydrantComponent.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DisconnectHydrantAction::ACTION_ID = "em5::DisconnectHydrantAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DisconnectHydrantAction::DisconnectHydrantAction() :
		Action(ACTION_ID),
		mTargetEntityId(qsf::getUninitialized<uint64>()),
		mCurrentState(STATE_INIT),
		mUseGroundHydrant(false)
	{
		// Nothing to do in here
	}

	DisconnectHydrantAction::~DisconnectHydrantAction()
	{
		// Nothing to do in here
	}

	void DisconnectHydrantAction::init(const qsf::Entity& entity)
	{
		mTargetEntityId = entity.getId();

		HydrantComponent* hydrantComponent = entity.getComponent<HydrantComponent>();
		if (nullptr != hydrantComponent)
		{
			mUseGroundHydrant = (hydrantComponent->getHydrantType() == HydrantComponent::GROUND_HYDRANT);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DisconnectHydrantAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTargetEntityId);
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool DisconnectHydrantAction::onStartup()
	{
		// Get the caller and target entity
		const qsf::Entity& callerEntity = getEntity();
		const qsf::Entity* targetEntity = callerEntity.getMap().getEntityById(mTargetEntityId);
		if (nullptr == targetEntity)
			return false;

		// Make the distance check
		const float maxDistance = ActionRangeSpecsGroup::getInstanceSafe().getGotoHydrantDistance();
		const float currentDistance = qsf::game::DistanceHelper::getDistance(*targetEntity, callerEntity);
		if (currentDistance > maxDistance)
			return false;

		return true;
	}

	qsf::action::Result DisconnectHydrantAction::updateAction(const qsf::Clock& clock)
	{
		// Get the target entity instance
		qsf::Entity* hydrantEntity = getMap().getEntityById(mTargetEntityId);
		QSF_CHECK(nullptr != hydrantEntity, "The EM5 disconnect hydrant action was unable to retrieve the target entity instance", return qsf::action::RESULT_DONE);

		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Play animation
				AnimationHelper animationHelper(getEntity());
				mMeshAnimationChannel = animationHelper.playAnimation(animationHelper.getAnimationFirefighterInstallHydrant(mUseGroundHydrant), false, true);
				mMeshAnimationChannel->setSpeed(2.0f);		// Double speed, otherwise this is just too much waiting
				getComponent().pushAction<PlayAudioAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(audio::EM5_FIREFIGHTER_HOSE_01, hydrantEntity, true, false, false);

				mCurrentState = STATE_DELETE_NOZZLE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DELETE_NOZZLE:
			{
				static const qsf::Time HIDE_NOZZLE = qsf::Time::fromSeconds(1.0f);
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getTimePosition() <= HIDE_NOZZLE)
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				getComponent().pushAction<GetEquipmentAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init("");
				EntityHelper(getEntity()).disconnectFromHydrant();

				mCurrentState = STATE_CREATE_HOSE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_CREATE_HOSE:
			{
				static const qsf::Time SHOW_HOSE = qsf::Time::fromSeconds(4.0f);
				if (mMeshAnimationChannel.valid() && mMeshAnimationChannel->getTimePosition() <= SHOW_HOSE)
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				// Check what equipment is important for switching back
				std::string equipmentName = equipment::HOSE_ROLLED;
				CommandableComponent* commandableComponent = getEntity().getComponent<CommandableComponent>();
				if (nullptr != commandableComponent)
				{
					for (auto commandId : commandableComponent->getCommandIds())
					{
						if (commandId == GetHoseHistoricalCommand::PLUGINABLE_ID)
						{
							// Special case for historical unit
							equipmentName = equipment::HOSE_ROLLED_HISTORICAL;
						}
						else if (commandId == GetSwordCommand::PLUGINABLE_ID)
						{
							// Special case for guardsman unit, rolled hose for him
							equipmentName = equipment::POLICE_SWORD;
							handleHistoricWell(*hydrantEntity);
						}
					}
				}

				getComponent().pushAction<GetEquipmentAction>(getPriority(), qsf::action::INSERT_AT_FRONT).init(equipmentName);

				mCurrentState = STATE_DISCONNECT;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DISCONNECT:
			{
				AnimationHelper animationHelper(getEntity());
				if (animationHelper.isAnimationPlaying(animationHelper.getAnimationFirefighterInstallHydrant(mUseGroundHydrant)))
				{
					// Wait for ending of the animation
					return qsf::action::RESULT_CONTINUE;
				}

				handleConnectedRoadVehicle(*hydrantEntity);

				if (mUseGroundHydrant)
				{
					hydrantEntity->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.0f), true);
				}

				// Finish process
				break;
			}
		}
		return qsf::action::RESULT_DONE;
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void DisconnectHydrantAction::handleConnectedRoadVehicle(qsf::Entity& hydrantEntity)
	{
		// Check if hydrant is an vehicle type
		const qsf::LinkComponent* linkComponent = hydrantEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			RoadVehicleComponent* roadVehicleComponent = linkComponent->getComponentFromEntityOrLinkedParent<RoadVehicleComponent>();
			if (nullptr != roadVehicleComponent)
			{
				const qsf::Entity& roadVehicleEntity = roadVehicleComponent->getEntity();

				// Check if we can free the vehicle and close the door
				bool lastConnectedFireman = true;
				const qsf::LinkComponent* vehicleLinkComponent = roadVehicleEntity.getComponent<qsf::LinkComponent>();
				if (nullptr != vehicleLinkComponent)
				{
					for (const qsf::LinkComponent* childComponent : vehicleLinkComponent->getChildLinks())
					{
						// Found other hydrant in use
						HydrantComponent* hydrantComponent = childComponent->getComponentFromEntityOrLinkedChild<HydrantComponent>();
						if (nullptr != hydrantComponent && hydrantComponent->getEntityId() != hydrantEntity.getId() && hydrantComponent->isConnected())
						{
							lastConnectedFireman = false;
							break;
						}
					}
				}

				if (lastConnectedFireman)
				{
					// Allow the vehicle movement
					qsf::ai::NavigationComponent* navigationComponent = roadVehicleEntity.getComponent<qsf::ai::NavigationComponent>();
					if (nullptr != navigationComponent)
					{
						navigationComponent->setActive(true);
					}

					// Close the trunk door
					if (!roadVehicleComponent->areAllDoorsOfTypeInState(DoorComponent::DOORTYPE_TRUNK, DoorComponent::DOOR_CLOSED))
					{
						roadVehicleComponent->openDoor(DoorComponent::DOORTYPE_TRUNK, false);
					}
				}
			}
		}
	}

	void DisconnectHydrantAction::handleHistoricWell(const qsf::Entity& hydrantEntity) const
	{
		// Start pump animation for historic wells
		const qsf::LinkComponent* linkComponent = hydrantEntity.getComponent<qsf::LinkComponent>();
		if (nullptr != linkComponent)
		{
			HistoricWellComponent* historicWellComponent = linkComponent->getComponentFromEntityOrLinkedParent<HistoricWellComponent>();
			if (nullptr != historicWellComponent)
			{
				historicWellComponent->stopPumpAnimation();
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
