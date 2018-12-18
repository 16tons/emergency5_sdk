// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/command/mode/debug/ChangeHealthCommandMode.h"
#include "em5/command/CommandContext.h"
#include "em5/component/vehicle/VehicleComponent.h"
#include "em5/freeplay/event/FreeplayEventManager.h"
#include "em5/freeplay/FreeplaySystem.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/health/injury/Injuries.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"
#include "em5/map/CollisionHelper.h"
#include "em5/plugin/Messages.h"
#include "em5/EM5Helper.h"

#include <qsf/debug/request/TextDebugDrawRequest.h>
#include <qsf/map/Entity.h>
#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>
#include <qsf/message/MessageSystem.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ChangeHealthCommandMode::PLUGINABLE_ID = qsf::StringHash("em5::ChangeHealthCommandMode");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ChangeHealthCommandMode::ChangeHealthCommandMode() :
		DebugCommandMode(PLUGINABLE_ID),
		mHealthPercentage(1.0f),
		mLifePercentage(0.25f)
	{
		// Nothing here
	}

	ChangeHealthCommandMode::~ChangeHealthCommandMode()
	{
		// Nothing here
	}

	void ChangeHealthCommandMode::changePercentages(float health, float life)
	{
		mHealthPercentage = health;
		mLifePercentage = life;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	void ChangeHealthCommandMode::startup()
	{
		// Nothing here
	}

	void ChangeHealthCommandMode::shutdown()
	{
		mDebugDrawProxy.unregister();
	}

	bool ChangeHealthCommandMode::executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds)
	{
		// Is there a target entity selected at all?
		if (nullptr != commandContext.mTargetEntity)
		{
			qsf::Entity& targetEntity = *commandContext.mTargetEntity;
			const EntityHelper entityHelper(targetEntity);

			// Is target outside?
			if (nullptr != entityHelper.getContainerEntity())
				return false;

			// Is target a person?
			if (entityHelper.isEntityPerson())
			{
				HealthComponent& healthComponent = targetEntity.getComponentSafe<HealthComponent>();

				switch (healthComponent.getState())
				{
					case HealthComponent::STATE_HEALTHY:
					{
						if (mLifePercentage < 0.0f)
						{
							if (nullptr == healthComponent.getInjury())
							{
								// Give new injury
								uint32 injuryId;
								if (CollisionHelper(targetEntity.getMap()).isInWater(targetEntity))
								{
									injuryId = injury::DROWNING_NORMAL;
								}
								else
								{
									static const std::vector<qsf::StringHash> possibleInjuries =
									{
										injury::BURN_OF_BREATH,
										injury::SMOKE_POISONING,
										injury::BURN_4_GRADE,
										injury::HEARTATTACK,
										injury::BROKEN_ARM
									};
									injuryId = qsf::Random::getAnyOf(possibleInjuries);
								}

								// Get freeplay event from the event manager
								FreeplayEvent* freeplayEvent = nullptr;
								const FreeplayEventManager::EventMap& events = EM5_FREEPLAY.getEventManager().getCurrentEvents();
								if (!events.empty())
								{
									// Use the first event for adding
									freeplayEvent = events.begin()->second;
								}

								healthComponent.applyInjuryById(injuryId, freeplayEvent);
							}
							else
							{
								// Injured but healthy means patient is standing/ contaminated
								healthComponent.setHealthEnergy(healthComponent.getHealthEnergy() + mLifePercentage * healthComponent.getMaximumHealthEnergy());

								// Set health very low, so health component can set following injury correctly
								if (healthComponent.getHealthEnergy() <= 0)
								{
									healthComponent.setHealthEnergy(1);
								}
							}
						}
						else
						{
							healthComponent.setHealthEnergy(healthComponent.getHealthEnergy() + mHealthPercentage * healthComponent.getMaximumHealthEnergy());
						}
						break;
					}

					case HealthComponent::STATE_INJURED:
					{
						if (!healthComponent.isInjured() && mLifePercentage > 0.0f)
						{
							// When target is nearly healed while getting healed, he gets status upgrade
							healthComponent.cleanup();
						}
						else
						{
							healthComponent.setLifeEnergy(healthComponent.getLifeEnergy() + mLifePercentage * healthComponent.getMaximumLifeEnergy());

							// Make health bar visible
							healthComponent.setDiagnosisComplete(true);

							if (healthComponent.getLifeEnergy() >= healthComponent.getMaximumLifeEnergy())
							{
								// Make them ready for transport
								healthComponent.setInjuredHealed();
								healthComponent.doctorTreatmentFinished();

								// Emit healing message like the doctor does, this is needed to support using this function in events and update objectives
								QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_HEALTH_TREATED_PERSON, targetEntity.getId()));
							}
						}
						break;
					}

					case HealthComponent::STATE_DEATHFIGHT:
					{
						if (healthComponent.getDeathEnergy() / healthComponent.getMaximumDeathEnergy() > 0.9f && mLifePercentage > 0.0f)
						{
							// When target is nearly healed while getting healed, he gets status upgrade
							// But setStatus does not exist
						}
						else
						{
							// Set to min or max death energy
							float newDeathEnergy = (mLifePercentage < 0.0f ? 1.0f : 0.99f * healthComponent.getMaximumDeathEnergy());
							healthComponent.setDeathEnergy(newDeathEnergy);
						}
						break;
					}

					case HealthComponent::STATE_DEAD:
					{
						if (mLifePercentage > 0.0f)
						{
							healthComponent.cleanup();
						}
						else
						{
							// Don't do anything, he can't get worse anyway
						}
						break;
					}
				}
			}
			else
			{
				qsf::DebugTextParameters params;
				params.setHorizontalAlignment(qsf::DebugTextParameters::H_ALIGNMENT_RIGHT);

				mDebugDrawProxy2.registerAt(QSF_DEBUGDRAW);

				// Is target a vehicle?
				VehicleComponent* vehicleComponent = targetEntity.getComponent<VehicleComponent>();
				if (nullptr != vehicleComponent && vehicleComponent->getDamageState() < DamageComponent::DAMAGESTATE_DAMAGE_LEVEL2)
				{
					vehicleComponent->applyDamage(-mLifePercentage);
					const int lifeEnergyPercent = (int)(vehicleComponent->getLifeEnergy() * 100.0f + 0.5f);
					mDebugDrawProxy2.addRequest(qsf::TextDebugDrawRequest("DAMAGED: " + std::to_string(lifeEnergyPercent) + "%", getMousePosition(), params));
				}
				else
				{
					// Is target any kind of damageable object?
					DamageComponent* damageComponent = targetEntity.getComponent<DamageComponent>();
					if (nullptr != damageComponent)
					{
						if (mLifePercentage < 0.0f)
						{
							switch (damageComponent->getDamageState())
							{
								case DamageComponent::DAMAGESTATE_INTACT:
									damageComponent->setDamageState(DamageComponent::DAMAGESTATE_DAMAGE_LEVEL1);
									mDebugDrawProxy2.addRequest(qsf::TextDebugDrawRequest("DAMAGE_LEVEL1", getMousePosition(), params));
									break;

								case DamageComponent::DAMAGESTATE_DAMAGE_LEVEL1:
									damageComponent->setDamageState(DamageComponent::DAMAGESTATE_DAMAGE_LEVEL2);
									mDebugDrawProxy2.addRequest(qsf::TextDebugDrawRequest("DAMAGE_LEVEL2", getMousePosition(), params));
									break;

								case DamageComponent::DAMAGESTATE_DAMAGE_LEVEL2:
									damageComponent->setDamageState(DamageComponent::DAMAGESTATE_DESTROYED);
									mDebugDrawProxy2.addRequest(qsf::TextDebugDrawRequest("DESTROYED", getMousePosition(), params));
									break;

								case DamageComponent::DAMAGESTATE_DESTROYED:
									damageComponent->setDamageState(DamageComponent::DAMAGESTATE_INTACT);
									mDebugDrawProxy2.addRequest(qsf::TextDebugDrawRequest("INTACT", getMousePosition(), params));
									break;

								case DamageComponent::DAMAGESTATE_INVALID:
									// Nothing to do in this case
									break;
							}
						}
						else
						{
							damageComponent->setDamageState(DamageComponent::DAMAGESTATE_INTACT);
							mDebugDrawProxy2.addRequest(qsf::TextDebugDrawRequest("INTACT", getMousePosition(), params));
						}
					}
				}

				mDebugDrawProxy2.setLifetimeInSeconds(1.5f);
			}
		}

		// Always return false to keep the command mode alive
		return false;
	}

	void ChangeHealthCommandMode::updateCommandMode(const qsf::Clock& clock)
	{
		// Debug output at the player's mouse cursor
		std::string outputText = (mLifePercentage < 0.0f) ? "Hurt or damage" : "Heal or set intact";

		mDebugDrawProxy.registerAt(QSF_DEBUGDRAW);
		mDebugDrawProxy.addRequest(qsf::TextDebugDrawRequest(outputText, getMousePosition() + glm::vec2(16, 8)));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
