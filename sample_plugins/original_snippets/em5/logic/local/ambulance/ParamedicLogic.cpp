// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/ambulance/ParamedicLogic.h"
#include "em5/audio/AudioHelper.h"
#include "em5/action/ActionPriority.h"
#include "em5/plugin/Jobs.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/effect/FadeEffectComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/command/base/PlacePersonCommand.h"
#include "em5/command/ambulance/PickupPersonByParamedicCommand.h"
#ifndef ENDUSER
	#include "em5/debug/GameDebugGroup.h"
#endif
#include "em5/health/HealthComponent.h"
#include "em5/health/injury/Injuries.h"
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/freeplay/event/FreeplayEventManager.h"
#include "em5/freeplay/FreeplaySystem.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/settings/GameSettingsGroup.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/MapHelper.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/game/player/Player.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/units/UnitPool.h"
#include "em5/game/Game.h"
#include "em5/logic/local/EntityLogicHelper.h"
#include "em5/logic/local/ReserveLogic.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>
#include <qsf_game/component/base/SelectableComponent.h>
#include <qsf_game/component/base/BoneToBoneLinkComponent.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/map/Map.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/logic/gamelogic/GameLogicComponent.h>
#include <qsf/job/JobArguments.h>
#ifndef ENDUSER
	#include <qsf/debug/DebugDrawManager.h>
	#include <qsf/debug/request/TextDebugDrawRequest.h>
#endif
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ParamedicLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::ParamedicLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 ParamedicLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ParamedicLogic::ParamedicLogic() :
		CarryPersonLogic(GAMELOGIC_TYPE_ID)
	{
		// Nothing here
	}

	void ParamedicLogic::injuryParamedicTeam(const qsf::MessageParameters& parameters)
	{
		// Removing the team already planned?
		if (nullptr != getEntity()->getComponent<FadeEffectComponent>())
		{
			// Do nothing for now with the injury as paramedics should be split already.
			// Unless fading comes from somewhere else - which would be unfortunate-
			// But in that case they are invincible while fading which sounds fine as well.
			parameters.respond(true);
			return;
		}

		qsf::LinkComponent* linkComponent = getEntity()->getOrCreateComponent<qsf::LinkComponent>();
		if (linkComponent != nullptr)
		{
			// Contaminated are not separated but we have to ensure the medic's stand still and look contaminated
			uint32 injuryId;
			if (parameters.getParameter("injuryId", injuryId))
			{
				if (injuryId == injury::RADIOACTIVE_CONTAMINATION || injuryId == injury::CHEMICAL_CONTAMINATION)
				{
					const auto& childLinks = linkComponent->getChildLinks();
					for (auto& childLinkComponent : childLinks)
					{
						qsf::Entity& childEntity = childLinkComponent->getEntity();

						qsf::MetadataComponent* metadataComponent = childEntity.getOrCreateComponent<qsf::MetadataComponent>();
						if (metadataComponent->getTags() == "first_paramedic" || metadataComponent->getTags() == "second_paramedic")
						{
							AnimationHelper medicAnimationHelper(childEntity);
							medicAnimationHelper.playAnimation(medicAnimationHelper.getAnimationContaminatedIdleLoop(), true);
						}
					}

					return;
				}
			}

			// Chance let the first or the second paramedic be the person how is important for the "blocking" value of the unit pool
			bool replaceOldMetaEntityWithFirstParamedic = qsf::Random::getRandomChance(0.5f);

			// Make a copy, for we will manipulate the list while iterating over it
			auto childLinksCopy = linkComponent->getChildLinks();

			// We test here for tags but it is also possible to use the array position to find the right entity
			for (auto& childLinkComponent : childLinksCopy)
			{
				qsf::Entity& childEntity = childLinkComponent->getEntity();

				qsf::MetadataComponent* metadataComponent = childEntity.getOrCreateComponent<qsf::MetadataComponent>();
				if (metadataComponent->getTags() == "stretcher")
				{
					// Is someone on the stretcher?
					qsf::LinkComponent* stretcherLinkComponent = childEntity.getComponent<qsf::LinkComponent>();
					if (nullptr != stretcherLinkComponent)
					{
						const auto& stretcherLinks = stretcherLinkComponent->getChildLinks();
						if (!stretcherLinks.empty())
						{
							// Unlink the lying person
							qsf::LinkComponent* lyingChildLink = *stretcherLinks.begin();
							qsf::Entity& lyingEntity = lyingChildLink->getEntity();
							lyingChildLink->setParentId(qsf::getUninitialized<uint64>());
							lyingEntity.destroyComponent<qsf::LinkComponent>();

							lyingEntity.destroyComponent<qsf::game::BoneToBoneLinkComponent>();

							// Delete the reservation to give other entities the possibility to interact with the target
							qsf::GameLogicComponent* gameLogicComponent = lyingEntity.getComponent<qsf::GameLogicComponent>();
							if (nullptr != gameLogicComponent)
							{
								ReserveParamedicLogic* reserveParamedicLogic = gameLogicComponent->getGameLogic<ReserveParamedicLogic>();
								if (nullptr != reserveParamedicLogic)
								{
									gameLogicComponent->deleteGameLogic(*reserveParamedicLogic);
								}
							}

							// Ensure injured person is placed on the ground
							EntityHelper(lyingEntity).applyPlacement();
							EntityHelper::teardownEntityAsGhost(lyingEntity);
						}
					}
				}
				else if (metadataComponent->getTags() == "first_paramedic" || metadataComponent->getTags() == "second_paramedic")
				{
					// First unlink the child
					childLinkComponent->setParentId(qsf::getUninitialized<uint64>());

					// Copy the components to our child
					childEntity.cloneComponent(getEntity()->getComponentSafe<qsf::MovableComponent>());
					childEntity.cloneComponent(getEntity()->getComponentSafe<PersonComponent>());
					childEntity.cloneComponent(getEntity()->getComponentSafe<qsf::ai::NavigationComponent>());
					childEntity.cloneComponent(getEntity()->getComponentSafe<CommandableComponent>());
					childEntity.createComponent<qsf::game::SelectableComponent>();
					childEntity.destroyComponent<qsf::LinkComponent>();

					metadataComponent->setName(getEntity()->getComponent<qsf::MetadataComponent>()->getName());
					if (metadataComponent->getTags() == "second_paramedic")
					{
						// The second medic has an other name
						childEntity.getComponentSafe<PersonComponent>().giveRandomName();
					}

					if ((replaceOldMetaEntityWithFirstParamedic && metadataComponent->getTags() == "first_paramedic") ||
						(!replaceOldMetaEntityWithFirstParamedic && metadataComponent->getTags() == "second_paramedic"))
					{
						Player* ownerPlayer = EM5_PLAYERS.getPlayerByIndex(childEntity.getComponent<CommandableComponent>()->getPlayerId());
						if (nullptr != ownerPlayer)
						{
							// Before the entity is added to the event, we change the internal unit, saved in the unit pool
							ownerPlayer->getUnitPool().replaceParamedicInUse(childEntity.getId(), getEntityId());
						}
					}

					// Copy the health component and initialize it correctly
					HealthComponent* childHealthComponent = static_cast<HealthComponent*>(childEntity.cloneComponent(getEntity()->getComponentSafe<HealthComponent>()));
					QSF_CHECK(nullptr != childHealthComponent, "Could not clone health component for paramedic", continue);

					// Injury the entity
					if (nullptr == childHealthComponent->getInjury())
					{
						uint32 injuryId = qsf::getUninitialized<uint32>();
						parameters.getParameter("injuryId", injuryId);

						uint32 freeplayEventId = qsf::getUninitialized<uint32>();
						parameters.getParameter("freeplayEventId", freeplayEventId);
						FreeplayEvent* event = EM5_FREEPLAY.getEventManager().getEventById(freeplayEventId);

						childHealthComponent->applyInjuryById(injuryId, event);
					}
				}
			}
		}

		// Now destroy the meta entity, it's not needed any more
		getEntity()->getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(qsf::Time::fromSeconds(1.f), true);

		parameters.respond(true);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool ParamedicLogic::onStartup()
	{
		// Register job
		// -> For performance reasons, do updates not in every tick, but in fixed time intervals
		qsf::jobs::JobConfiguration jobConfiguration;
		jobConfiguration.mTimeBetweenCalls = qsf::Time::fromSeconds(1.0f);
		mSimulationJobProxy.registerAt(Jobs::SIMULATION_LOGIC, boost::bind(&ParamedicLogic::updateSimulation, this, _1), jobConfiguration);

		mInjuryParamedicTeamProxy.registerAt(qsf::MessageConfiguration(Messages::EM5_HEALTH_PERSON_PRE_INJURED, getEntityId()), boost::bind(&ParamedicLogic::injuryParamedicTeam, this, _1));

		return true;
	}

	void ParamedicLogic::onShutdown()
	{
		// Unregister job
		mSimulationJobProxy.unregister();
		mInjuryParamedicTeamProxy.unregister();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ParamedicLogic::updateSimulation(const qsf::JobArguments& jobArguments)
	{
		// Access the caller's action plan
		const qsf::ActionComponent& actionComponent = getEntity()->getOrCreateComponentSafe<qsf::ActionComponent>();

		// Check if the paramedic is currently busy
		bool isBusy = (actionComponent.getCurrentPriority() >= action::AUTOMATISM_STD);	// Overwrite idle actions and low-priority automatisms

		qsf::MovableComponent* movableComponent = getEntity()->getComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent)
		{
			if (glm::length(movableComponent->getVelocity()) > glm::epsilon<float>() && EntityHelper(*getEntity()).getContainerEntity() == nullptr)
			{
				AudioHelper::playMedicStretcherMove(mAudioProxy, *getEntity());
			}
			else
			{
				mAudioProxy.stop();
			}
			mAudioProxy.synchronizeWithMultiplay();
		}

		if (!MapHelper::isUnitAutomatismActive())
		{
			// No automatism active
			isBusy = true;
		}

		// Check if the person is inside a road vehicle
		if (EntityHelper(*getEntity()).isEntityInRoadVehicle() || EntityHelper(*getEntity()).isPersonInjured())
		{
			isBusy = true;
		}

		if (isBusy)
		{
			// Reset wait time
			mTimeWaiting = qsf::Time::ZERO;
		}
		else
		{
			// Wait some time until getting active
			mTimeWaiting += jobArguments.getTimePassed();

			// Waited long enough now?
			if (mTimeWaiting >= qsf::Time::fromSeconds(LogicSpecsGroup::getInstanceSafe().getParamedicMinimumWaitingTime()))
			{
				// Perform the automatism now
				performAutomatism();

				// Reset wait time
				mTimeWaiting = qsf::Time::ZERO;
			}
		}
	}

	void ParamedicLogic::performAutomatism()
	{
		GameSettingsGroup::AutomatismValue automatismValue = GameSettingsGroup::getInstanceSafe().getAutomatismValue();
		if (automatismValue >= GameSettingsGroup::AUTOMATISM_ALL)
		{
			// Check if the paramedic is already carrying a person
			if (qsf::isInitialized(getCarryPerson()))
			{
				placePersonAutomatism();
			}
			else
			{
				pickupPersonAutomatism();
			}
		}
	}

	void ParamedicLogic::pickupPersonAutomatism()
	{
		// Get the transform component for the position
		const qsf::TransformComponent* transformComponent = getEntity()->getComponent<qsf::TransformComponent>();
		if (nullptr == transformComponent)
			return;

		glm::vec3 ownerPosition = transformComponent->getPosition();

		// Search for health components in the radius
		std::vector<HealthComponent*> healthComponentVector;
		qsf::ComponentMapQuery(getMap()).getInstancesInCircle(ownerPosition, LogicSpecsGroup::getInstanceSafe().getParamedicSearchRadius(), healthComponentVector);

		// Sort the list for the distance
		qsf::game::IsLeftComponentCloserToTargetPosition compareObject(ownerPosition);
		std::sort(healthComponentVector.begin(), healthComponentVector.end(), compareObject);

		for (HealthComponent* healthComponent : healthComponentVector)
		{
			if (EntityLogicHelper::checkAndExecuteAutomatism<PickupPersonByParamedicCommand>(getEntity(), &healthComponent->getEntity()))
			{
				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:PickupPersonByParamedic", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				break;
			}
		}
	}

	void ParamedicLogic::placePersonAutomatism()
	{
		// Get the transform component for the position
		const qsf::TransformComponent* transformComponent = getEntity()->getComponent<qsf::TransformComponent>();
		if (nullptr == transformComponent)
			return;

		const glm::vec3 ownerPosition = transformComponent->getPosition();

		// Search for vehicle components (RTW, IMT, rescue helicopter) in the radius
		boost::container::flat_multimap<float, qsf::Entity*> targetsByDistance;
		{
			std::vector<VehicleComponent*> vehicleComponentVector;
			qsf::ComponentMapQuery(getMap()).getInstancesInCircle(ownerPosition, LogicSpecsGroup::getInstanceSafe().getParamedicSearchRadius(), vehicleComponentVector);

			for (VehicleComponent* component : vehicleComponentVector)
			{
				qsf::Entity& vehicle = component->getEntity();
				float distance = glm::distance(ownerPosition, vehicle.getOrCreateComponentSafe<qsf::TransformComponent>().getPosition());

				CommandableComponent* commandableComponent = vehicle.getComponent<CommandableComponent>();
				if (nullptr != commandableComponent)
				{
					// Prefer the IMT as long as it is not too far away
					if (commandableComponent->isAmbulanceIMT())
					{
						distance -= 50.0f;		// May get negative, but who cares
					}

					targetsByDistance.emplace(distance, &vehicle);
				}
			}
		}

		for (auto& pair : targetsByDistance)
		{
			qsf::Entity* vehicle = pair.second;
			if (EntityLogicHelper::checkAndExecuteAutomatism<PlacePersonCommand>(getEntity(), vehicle))
			{
				#ifndef ENDUSER
					if (GameDebugGroup::getInstanceSafe().getShowAutomatismDebug())
					{
						QSF_DEBUGDRAW.requestDraw(qsf::TextDebugDrawRequest("auto:PlacePerson", ownerPosition, qsf::Color4::YELLOW), qsf::DebugDrawLifetimeData(qsf::Time::fromSeconds(5.0f)));
					}
				#endif
				return;
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
