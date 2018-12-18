// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/movement/PersonMagnetComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/effect/GroundSpotComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/command/base/EnterVehicleCommand.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/hint/HintSystem.h"
#include "em5/game/selection/SelectionManager.h"
#include "em5/game/Game.h"
#include "em5/logic/local/civilians/FollowMagnetLogic.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Jobs.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/audio/AudioHelper.h"
#include "em5/audio/AudioProxy.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/base/ScratchBuffer.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Anonymous namespace                                   ]
//[-------------------------------------------------------]
namespace
{
	const float UPDATE_FREQUENCY = 1.0f;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PersonMagnetComponent::COMPONENT_ID = qsf::StringHash("em5::PersonMagnetComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PersonMagnetComponent::PersonMagnetComponent(qsf::Prototype* prototype):
		qsf::Component(prototype),
		mMaxFollowers(5),
		mInfluenceRadius(0.0f),
		mPlaceDistMin(1.0f),
		mPlaceDistMax(3.0f),
		mTalkPause(20.0f),
		mMagnetCarrier(qsf::getUninitialized<uint64>()),
		mMaxForwardSpeedBackup(3.3f),
		mVisualizationRadius(0.0f)
	{
		// Nothing here
	}

	PersonMagnetComponent::~PersonMagnetComponent()
	{
		// Nothing here
	}

	void PersonMagnetComponent::enableVisualization(float visualizationRadius)
	{
		if (qsf::isInitialized(mMagnetCarrier) && visualizationRadius > 0.0f)
		{
			disableVisualization();

			mVisualizationRadius = visualizationRadius;

			const bool isSelected = EM5_GAME.getSelectionManager().isIdSelected(mMagnetCarrier);
			if (isSelected)
			{
				switchGroundSpot(true);
			}

			mOnSelectionChangeProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_CHANGE_SELECTION), boost::bind(&PersonMagnetComponent::onChangeSelection, this, _1));
		}
	}

	void PersonMagnetComponent::disableVisualization()
	{
		if (isVisualizationEnabled())
		{
			mVisualizationRadius = 0.0f;
			mOnSelectionChangeProxy.unregister();
			switchGroundSpot(false);
		}
	}

	bool PersonMagnetComponent::isVisualizationEnabled() const
	{
		return (mVisualizationRadius > 0.0f);
	}

	float PersonMagnetComponent::getVisualizationRadius() const
	{
		return mVisualizationRadius;
	}

	void PersonMagnetComponent::enableMagnet(float influenceRadius)
	{
		if (qsf::isInitialized(mMagnetCarrier) && influenceRadius > 0.0f)
		{
			disableMagnet();

			mPlaceDistMin = LogicSpecsGroup::getInstanceSafe().getPoliceMegaphonePlaceDistMin();
			mPlaceDistMax = LogicSpecsGroup::getInstanceSafe().getPoliceMegaphonePlaceDistMax();
			mTalkPause = LogicSpecsGroup::getInstanceSafe().getPoliceMegaphoneTalkPause();

			mInfluenceRadius = influenceRadius;
			setUpdateFrequency(qsf::Time::fromSeconds(UPDATE_FREQUENCY));

			// The person magnet component is usually linked to the equipment entity
			uint64 parentId = qsf::LinkComponent::getTopmostAncestorPrototype(getEntity(), qsf::LinkComponent::TRANSFORM).getId();

			// If equipment is hidden the component is also removed. So once equipment gets hidden more often those proxy's would not be necessary (but also won't hurt).
			mOnInjuredProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_GETSINJURED_PERSON, parentId), boost::bind(&PersonMagnetComponent::cancelMagnet, this, _1));
			mOnContaminatedProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_COMPONENT_HEALTH_PERSON_CONTAMINATED, parentId), boost::bind(&PersonMagnetComponent::cancelMagnet, this, _1));
			mOnEnterVehicleProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_POST_COMMAND_EXECUTE, EnterVehicleCommand::PLUGINABLE_ID, parentId), boost::bind(&PersonMagnetComponent::cancelMagnet, this, _1));

			// For show the megaphone effect on the ground
			enableVisualization(influenceRadius);

			// Play audio
			AudioProxy audioProxy;
			AudioHelper::playPoliceMegaphone(audioProxy, getEntity());
			audioProxy.synchronizeWithMultiplay();
			audioProxy.detach();

			qsf::Entity* carrierEntity = getEntity().getMap().getEntityById(mMagnetCarrier);
			if (nullptr != carrierEntity)
			{
				AnimationHelper(*carrierEntity).clearAnimation();

				// Slower the movement of the owner
				qsf::ai::NavigationComponent* navigationComponent = carrierEntity->getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					mMaxForwardSpeedBackup = navigationComponent->getNativeMaxForwardSpeed();
					static const float MOVEMENTSPEED_WITH_ACTIVE_MEGAPHONE = 1.5f;
					navigationComponent->setNativeMaxForwardSpeed(MOVEMENTSPEED_WITH_ACTIVE_MEGAPHONE);
				}
			}
		}
	}

	void PersonMagnetComponent::disableMagnet()
	{
		if (isMagnetEnabled())
		{
			qsf::Entity* carrierEntity = getEntity().getMap().getEntityById(mMagnetCarrier);
			if (nullptr != carrierEntity)
			{
				AnimationHelper(*carrierEntity).clearAnimation();

				// Restore the final saved movement speed
				qsf::ai::NavigationComponent* navigationComponent = carrierEntity->getComponent<qsf::ai::NavigationComponent>();
				if (nullptr != navigationComponent)
				{
					navigationComponent->setNativeMaxForwardSpeed(mMaxForwardSpeedBackup);
				}
			}

			disableVisualization();

			mOnInjuredProxy.unregister();
			mOnContaminatedProxy.unregister();
			mOnEnterVehicleProxy.unregister();
			mUpdateJobProxy.unregister();
			mInfluenceRadius = 0.0f;	// Because we have the "isMagnetEnabled" check sticked to this value, we need to reset it here
			removeFollowers();
		}
	}

	bool PersonMagnetComponent::isMagnetEnabled() const
	{
		return (mInfluenceRadius > 0.0f);
	}

	float PersonMagnetComponent::getInfluenceRadius() const
	{
		return mInfluenceRadius;
	}

	void PersonMagnetComponent::setCarrierEntityId(uint64 entityId)
	{
		if (mMagnetCarrier != entityId)
		{
			// Remember the previous state of the magnet
			const float magnetRadius = getInfluenceRadius();
			disableMagnet();

			// Even if the magnet wasn't enabled before, the visualization might still have been (for multiplayer clients)
			const float visualizationRadius = getVisualizationRadius();
			disableVisualization();

			mMagnetCarrier = entityId;

			if (visualizationRadius > 0.0f)
			{
				enableVisualization(visualizationRadius);
			}

			if (magnetRadius > 0.0f)
			{
				enableMagnet(magnetRadius);
			}
		}
	}

	uint64 PersonMagnetComponent::getCarrierEntityId() const
	{
		return mMagnetCarrier;
	}

	void PersonMagnetComponent::setMaxFollowers(uint32 maxFollowers)
	{
		mMaxFollowers = maxFollowers;
	}

	uint32 PersonMagnetComponent::getMaxFollowers() const
	{
		return mMaxFollowers;
	}

	glm::vec3 PersonMagnetComponent::getFollowOffset(uint64 followerEntityId, bool localCoordinates, bool recalculateOffset)
	{
		glm::vec3 offset = mFollowerOffsets[followerEntityId];

		const qsf::TransformComponent& transformComponentMagnet = getEntity().getOrCreateComponentSafe<qsf::TransformComponent>();
		if (recalculateOffset)
		{
			qsf::Entity* personEntity = getEntity().getMap().getEntityById(followerEntityId);
			if (nullptr != personEntity)
			{
				// Updating offset each time is not really necessary, but looks more natural (persons walk less)
				const qsf::TransformComponent& transformComponentFollower = personEntity->getOrCreateComponentSafe<qsf::TransformComponent>();
				offset = updateFollowerOffset(transformComponentMagnet, transformComponentFollower);
				mFollowerOffsets[followerEntityId] = offset;
			}
		}

		if (localCoordinates)
		{
			return offset;
		}

		// Deliberately no rotation transformation for the offset. Persons should _not_ walk to the other side when the squad view direction changes...
		return transformComponentMagnet.getPosition() + offset;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void PersonMagnetComponent::onSetSimulating(bool simulating)
	{
		// Register for update
		if (simulating && isActive())
		{
			setUpdateFrequency(qsf::Time::fromSeconds(UPDATE_FREQUENCY));
		}
		else
		{
			mUpdateJobProxy.unregister();
			mOnEnterVehicleProxy.unregister();
			mOnEnterVehicleProxy.unregister();
			mOnContaminatedProxy.unregister();
			mOnSelectionChangeProxy.unregister();
		}
	}

	void PersonMagnetComponent::onShutdown()
	{
		disableMagnet();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void PersonMagnetComponent::removeFollowers()
	{
		const qsf::Map& map = getEntity().getMap();
		for (auto follower : mFollowerOffsets)
		{
			qsf::Entity* followerEntity = map.getEntityById(follower.first);
			if (followerEntity != nullptr)
			{
				qsf::GameLogicComponent& gameLogicComponent = followerEntity->getOrCreateComponentSafe<qsf::GameLogicComponent>();
				FollowMagnetLogic * followMagnetLogic = gameLogicComponent.getGameLogic<FollowMagnetLogic>();
				if (followMagnetLogic != nullptr)
				{
					gameLogicComponent.deleteGameLogic(*followMagnetLogic);
				}
			}
		}
		mFollowerOffsets.clear();
	}

	void PersonMagnetComponent::switchGroundSpot(bool activate)
	{
		const qsf::Map& map = getEntity().getMap();
		qsf::Entity* carrierEntity = map.getEntityById(mMagnetCarrier);
		if (nullptr != carrierEntity)
		{
			if (activate)
			{
				// Should never be running at this point in time, but be safe
				GroundSpotComponent* groundSpotComponent = carrierEntity->getOrCreateComponent<GroundSpotComponent>(false);
				if (nullptr != groundSpotComponent && !groundSpotComponent->isRunning())
				{
					groundSpotComponent->setRadius(mVisualizationRadius);
					groundSpotComponent->setGroundSpotLightHeight(30.0f);
					groundSpotComponent->setClipFactors(0.8f, 1.3f);
					groundSpotComponent->setSpotLightTexture("em5/texture/default/groundmarker_radius_megaphone");

					// Now that the ground spot component is configured, start it
					groundSpotComponent->startup(map.isRunning());
				}
			}
			else
			{
				// Don't just deactivate the ground spot component, get rid of it all together to avoid nasty
				// bugs like policemen leaving a car and having a ground spot enabled without megaphone
				carrierEntity->destroyComponent<GroundSpotComponent>();
			}
		}
	}

	void PersonMagnetComponent::setUpdateFrequency(qsf::Time updateInterval)
	{
		qsf::jobs::JobConfiguration jobConfig;
		jobConfig.mTimeBetweenCalls = updateInterval;
		mUpdateJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&PersonMagnetComponent::updateJob, this, _1), jobConfig);
	}

	glm::vec3 PersonMagnetComponent::updateFollowerOffset(const qsf::TransformComponent& magnetTransform, const qsf::TransformComponent& followerTransform) const
	{
		// Keep some offset, distance random but keep the direction the person came from
		glm::vec3 offset(followerTransform.getPosition() - magnetTransform.getPosition());
		float oldLen = glm::length(offset);
		if (oldLen > mPlaceDistMax)
		{
			offset = glm::normalize(offset) * qsf::Random::getRandomFloat(mPlaceDistMin, mPlaceDistMax);
		}
		return offset;
	}

	void PersonMagnetComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		if (mInfluenceRadius <= 0.f)
			return;

		if (mFollowerOffsets.size() >= static_cast<size_t>(mMaxFollowers))
			return;

		qsf::Entity& magnetEntity = getEntity();
		const qsf::TransformComponent& transformComponent = magnetEntity.getOrCreateComponentSafe<qsf::TransformComponent>();

		// Search for other persons in range
		qsf::StdVectorScratchBuffer<PersonComponent*> personComponents;
		qsf::ComponentMapQuery(magnetEntity.getMap()).getInstancesInCircle(transformComponent.getPosition(), mInfluenceRadius, *personComponents);
		for (PersonComponent* personComponent : personComponents)
		{
			qsf::Entity& personEntity = personComponent->getEntity();

			// Flagged to ignore megaphone
			if (personComponent->getPersonFlags(PersonComponent::FLAG_IGNORE_MEGAPHONE))
				return;

			// Got person already
			if (mFollowerOffsets.find(personEntity.getId()) != mFollowerOffsets.end())
				continue;

			// Not a person we can convince to follow
			// TODO(fw): Now that we have "PersonComponent::FLAG_IGNORE_MEGAPHONE", maybe we could get rid of some of these checks?
			EntityHelper personEntityHelper(personEntity);
			if (!personEntityHelper.isCivilPerson() ||
				personEntityHelper.isGangsterPerson() ||
				personEntityHelper.isPersonBuried() ||
				personEntityHelper.isPersonInjured() ||
				personEntityHelper.isArrested() ||
				personEntityHelper.isPersonContaminated())
			{
				continue;
			}

			// Already following another megaphone?
			qsf::GameLogicComponent& gameLogicComponent = personEntity.getOrCreateComponentSafe<qsf::GameLogicComponent>();
			FollowMagnetLogic* followMagnetLogic = gameLogicComponent.getGameLogic<FollowMagnetLogic>();
			if (followMagnetLogic != nullptr)
			{
				// If so then check which is closer and take this
				qsf::Entity* entity = getEntity().getMap().getEntityById(followMagnetLogic->getTargetMagnetEntityId());
				if (nullptr != entity)
				{
					const glm::vec3 lastPosition = entity->getComponentSafe<qsf::TransformComponent>().getPosition();
					const glm::vec3 newPosition = transformComponent.getPosition();

					const float distanceToLastMagnet = qsf::game::DistanceHelper::get2dDistance(lastPosition, personEntity);
					const float distanceToNewMagnet = qsf::game::DistanceHelper::get2dDistance(newPosition, personEntity);

					// If the current (new) magnet is closer to the person change the magnet target
					if (distanceToLastMagnet > distanceToNewMagnet)
					{
						PersonMagnetComponent* lastPersonMagnetComponent = entity->getComponent<PersonMagnetComponent>();
						if (nullptr != lastPersonMagnetComponent)
						{
							// Delete person from the old
							lastPersonMagnetComponent->mFollowerOffsets.erase(personEntity.getId());
						}
					}

					// Else do nothing
					else
					{
						continue;
					}
				}
			}

			// Follow me my disciple
			followMagnetLogic = gameLogicComponent.getOrCreateGameLogic<FollowMagnetLogic>();
			followMagnetLogic->setTargetMagnet(magnetEntity.getId());

			// Disable warbler icon
			StatusOverlayComponent::showIcon(personEntity, StatusOverlayComponent::STATUS_ICON_WARBLERTWEET, false);

			// Remember follower
			const qsf::TransformComponent& transformComponentFollower = personEntity.getOrCreateComponentSafe<qsf::TransformComponent>();
			mFollowerOffsets[personEntity.getId()] = updateFollowerOffset(transformComponent, transformComponentFollower);

			// Make announcements
			const qsf::Time& timeNow = jobArguments.getClock().getCurrentTime();
			if ((timeNow - mTimeLastAnnouncement).getSeconds() > mTalkPause)
			{
				mTimeLastAnnouncement = timeNow;
				// TODO(db): Soundfile doesnt exist but should. Maybe add soundfile in patch
				//EM5_GAME.getHintSystem().addMessage("ID_MEGAPHONE_LEAD_01", HintMessage::MESSAGETYPE_UNITHINT);
			}
		}
	}

	void PersonMagnetComponent::cancelMagnet(const qsf::MessageParameters& parameters)
	{
		disableMagnet();
	}

	void PersonMagnetComponent::onChangeSelection(const qsf::MessageParameters& parameters)
	{
		// Toggle the visibility of the police megaphone
		const bool isSelected = EM5_GAME.getSelectionManager().isIdSelected(mMagnetCarrier);
		switchGroundSpot(isSelected);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
