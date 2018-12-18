// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/BombComponent.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/audio/AudioHelper.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/damage/DamageComponent.h"
#include "em5/component/event/EventIdComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/debug/GameDebugGroup.h"
#include "em5/freeplay/eventspread/EventSpreading.h"
#include "em5/map/EntityHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/plugin/Jobs.h"
#include "em5/EM5Helper.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/link/link/prototype/ContainerLink.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/map/query/ComponentMapQuery.h>
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
	const uint32 BombComponent::COMPONENT_ID = qsf::StringHash("em5::BombComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	BombComponent::BombComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mTimeRunning(qsf::Time::ZERO),
		mTimeToDefuse(qsf::Time::ZERO),
		mCurrentTimeDefuse(qsf::Time::ZERO),
		mState(STATE_INACTIVE),
		mIsDefuseActive(false)
	{
		// Nothing to do in here
	}

	BombComponent::~BombComponent()
	{
		// Nothing to do in here
	}

	qsf::Time BombComponent::getTimeRunning() const
	{
		return mTimeRunning;
	}

	void BombComponent::setTimeRunning(qsf::Time timeRunning)
	{
		mTimeRunning = timeRunning;
	}

	qsf::Time BombComponent::getTimeToDefuse() const
	{
		return mTimeToDefuse;
	}

	void BombComponent::setTimeToDefuse(qsf::Time timeToDefuse)
	{
		mTimeToDefuse = timeToDefuse;
	}

	bool BombComponent::getIsDefuseBombActive() const
	{
		return mIsDefuseActive;
	}

	void BombComponent::setIsDefuseBombActive(bool defuseBomb)
	{
		mIsDefuseActive = defuseBomb;
	}

	bool BombComponent::isBombActive() const
	{
		return (mState == STATE_ACTIVE);
	}

	bool BombComponent::isBombDefused() const
	{
		return (mState == STATE_DEFUSED);
	}

	bool BombComponent::isBombIntact() const
	{
		return (mState != STATE_EXPLODED);
	}

	bool BombComponent::canBeDefused() const
	{
		return isBombActive();
	}

	void BombComponent::startBombTimer(FreeplayEvent& freeplayEvent, bool playTicking)
	{
		mState = STATE_ACTIVE;

		EventIdComponent* eventIdComponent = getEntity().getComponent<EventIdComponent>();
		if (nullptr != eventIdComponent)
		{
			eventIdComponent->setEvent(freeplayEvent, em5::eventspreadreason::EVENT_SPECIFIC);
		}

		enableTicking(playTicking);

		// Register icon (status + minimap)
		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_BOMB, true);

		mCurrentTimeDefuse = mTimeToDefuse;

		activateJob();
	}

	void BombComponent::explodeBomb()
	{
		mTimeRunning = qsf::Time::ZERO;
		mState = STATE_EXPLODED;

		damageObjectsAroundBomb();

		// Start explosion effect
		DamageComponent* damageComponent = getEntity().getComponent<DamageComponent>();
		if (nullptr != damageComponent)
		{
			damageComponent->showExplosionFireParticles();
		}

		// Unregister icon (status + minimap)
		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_BOMB, false);

		// Play explosion audio
		AudioHelper::playBombExplosion(mAudioProxy, getEntity());
		mAudioProxy.detach();
		mAudioProxy.synchronizeWithMultiplay();

		qsf::MessageParameters parameters;
		parameters.setParameter("ExplodedBomb", getEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ENTITY_BOMB_EXPLODED, getEntityId()), parameters);

		deactivateJob();

		// Tell the event spreading about it, so the "dispose bomb" objectives will be removed
		EventSpreading::bombExplosionReaction(getEntity());
	}

	void BombComponent::defuseBomb()
	{
		// Send message for defused bomb
		qsf::MessageParameters parameters;
		parameters.setParameter("DefusedBomb", getEntityId());
		QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ENTITY_BOMB_DEFUSED, getEntityId()), parameters);

		mAudioProxy.stop();

		mState = STATE_DEFUSED;

		// Unregister icon (status + minimap)
		StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_BOMB, false);

		// Hide progress bar
		StatusOverlayComponent::hideBarOverlay(getEntity(), StatusOverlayComponent::BAR_TYPE_PROGRESS);

		deactivateJob();
	}

	void BombComponent::enableTicking(bool enable)
	{
		if (enable)
		{
			AudioHelper::playBombTicking(mAudioProxy, getEntity());
		}
		else
		{
			mAudioProxy.stop();
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void BombComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mTimeRunning);
		serializer.serialize(mTimeToDefuse);
		serializer.serialize(mIsDefuseActive);
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void BombComponent::activateJob()
	{
		// Is job already active?
		if (!mUpdateJobProxy.isValid())
		{
			// Activate job by registering via the job proxy
			mUpdateJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&BombComponent::updateJob, this, _1));
		}
	}

	void BombComponent::deactivateJob()
	{
		// Stop audio
		mAudioProxy.stop();
		mAudioProxy.synchronizeWithMultiplay();

		mUpdateJobProxy.unregister();
		mDebugDrawProxy.unregister();
	}

	void BombComponent::updateJob(const qsf::JobArguments& jobArguments)
	{
		if (mState != STATE_ACTIVE)
		{
			// No update in other states
			deactivateJob();
			return;
		}

		// Countdown timer
		{
			mTimeRunning -= jobArguments.getTimePassed();
			if (mTimeRunning <= qsf::Time::ZERO)
			{
				explodeBomb();
				return;
			}
		}

		// Update defuse process
		if (mIsDefuseActive)
		{
			mCurrentTimeDefuse -= jobArguments.getTimePassed();
			if (mCurrentTimeDefuse <= qsf::Time::ZERO)
			{
				defuseBomb();
				return;
			}
			else
			{
				// Update progress bar
				StatusOverlayComponent::showBarOverlay(getEntity(), StatusOverlayComponent::BAR_TYPE_PROGRESS, 1.0f - mCurrentTimeDefuse / mTimeToDefuse);
			}
		}

#ifndef ENDUSER
		drawDebugDisplay();
#endif
	}

	void BombComponent::damageObjectsAroundBomb()
	{
		// TODO(mk) This function is moved from "BomberEvent". Don't take this code here to serious
		// Here we ignite everything in explosion radius. Imho the bomb needs a right fire material settings and should do damage to everything,
		// depeding on the firematerial setting.

		qsf::ComponentMapQuery componentMapQuery(getEntity().getMap());
		glm::vec3 bombPosition = getEntity().getOrCreateComponent<qsf::TransformComponent>()->getPosition();
		const float explosionRadius = 20.f;

		FreeplayEvent* freeplayEvent = EntityHelper(getEntity()).getFreeplayEvent();	// Can be nullptr

		{ // Injure all people in range
			std::vector<PersonComponent*> personComponents;
			componentMapQuery.getInstancesInCircle<PersonComponent>(bombPosition, explosionRadius, personComponents);

			for (PersonComponent* personComponent : personComponents)
			{
				qsf::Entity& personEntity = personComponent->getEntity();
				EntityHelper entityHelper(personEntity);
				if (nullptr == entityHelper.getContainerEntity())
				{
					if (entityHelper.startFire(freeplayEvent))
					{
						if (nullptr != freeplayEvent)
						{
							personEntity.getOrCreateComponentSafe<EventIdComponent>().setEvent(*freeplayEvent, eventspreadreason::INJURY);
						}
					}
				}
			}
		}

		{ // Damage all vehicles in range
			std::vector<VehicleComponent*> vehicleComponents;
			componentMapQuery.getInstancesInCircle<VehicleComponent>(bombPosition, explosionRadius, vehicleComponents);

			for (VehicleComponent* vehicleComponent : vehicleComponents)
			{
				qsf::Entity& vehicleEntity = vehicleComponent->getEntity();
				if (EntityHelper(vehicleEntity).startFire(freeplayEvent))
				{
					if (nullptr != freeplayEvent)
					{
						vehicleEntity.getOrCreateComponentSafe<EventIdComponent>().setEvent(*freeplayEvent, eventspreadreason::FIRE);
					}
				}
			}
		}

		{ // Damage all buildings in range
			std::vector<BuildingComponent*> buildingComponents;
			componentMapQuery.getInstancesInCircle<BuildingComponent>(bombPosition, explosionRadius, buildingComponents);

			for (BuildingComponent* buildingComponent : buildingComponents)
			{
				qsf::Entity& buildingEntity = buildingComponent->getEntity();
				if (EntityHelper(buildingEntity).startFire(freeplayEvent))
				{
					if (nullptr != freeplayEvent)
					{
						buildingEntity.getOrCreateComponentSafe<EventIdComponent>().setEvent(*freeplayEvent, eventspreadreason::FIRE);
					}

					for (auto& iterator : buildingComponent->getLinks())
					{
						const qsf::ContainerLink& containerLink = *iterator.second;
						qsf::Entity& linkedEntity = containerLink.getTargetEntity();

						if (!EntityHelper(linkedEntity).isEntityPerson())
						{
							// An other child entity, maybe light or something else
							continue;
						}
						linkedEntity.getOrCreateComponent<qsf::ActionComponent>()->pushAction<ExitBuildingAction>(action::COMMAND_HIGH, qsf::action::INSERT_AT_FRONT);
						break;
					}
				}
			}
		}
	}

	void BombComponent::drawDebugDisplay()
	{
		const bool showDebug = GameDebugGroup::getInstanceSafe().getShowEventDebug() || canBeDefused();
		if (showDebug)
		{
			mDebugDrawProxy.registerForComponent(*this);
			const qsf::TransformComponent* transformComponent = getEntity().getComponent<qsf::TransformComponent>();
			if (nullptr == transformComponent)
				return;

			const glm::vec3& position = transformComponent->getPosition();

			std::stringstream builder;
			builder << "Bombtimer: " << (int)mTimeRunning.getSeconds();
			if (mTimeToDefuse != mCurrentTimeDefuse)
			{
				builder << " defusetime: " << (int)mCurrentTimeDefuse.getSeconds();
			}
			mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(builder.str(), position));
		}
		else
		{
			mDebugDrawProxy.unregister();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
