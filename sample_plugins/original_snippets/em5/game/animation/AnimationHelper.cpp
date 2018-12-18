// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/component/vehicle/HelicopterComponent.h"
#include "em5/component/vehicle/parts/VehicleCraneComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/action/move/MoveAction.h"
#include "em5/map/EntityHelper.h"
#include "em5/ai/MovementModes.h"
#include "em5/game/units/UnitHelper.h"

#include <qsf_game/equipment/EquipmentComponent.h>

#include <qsf_ai/navigation/NavigationComponent.h>

#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/move/MovableComponent.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const std::string ANIMATIONPATH_PERSON =	"em5/skeleton/person/";
	const std::string ANIMATIONPATH_DEFAULT =	"em5/skeleton/default/";
	const std::string ANIMATIONPATH_EQUIPMENT =	"em5/skeleton/equipment/";
	const std::string ANIMATIONPATH_DOCTOR =	"em5/skeleton/doctor/";
	const std::string ANIMATIONPATH_DIVER =	    "em5/skeleton/diver/";
	const std::string ANIMATIONPATH_INJURY =	"em5/skeleton/injury/";
	const std::string ANIMATIONPATH_DROWN =		"em5/skeleton/drown/";
	const std::string ANIMATIONPATH_VEHICLE =	"em5/skeleton/vehicle/";
	const std::string ANIMATIONPATH_ANIMAL =	"em5/skeleton/animal/";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	bool AnimationHelper::isAnimationPlaying(const std::string& animationName)
	{
		const qsf::MeshAnimationComponent* meshAnimationComponent = mEntity.getComponent<qsf::MeshAnimationComponent>();
		return (nullptr != meshAnimationComponent && meshAnimationComponent->isAnimationPlaying(qsf::AssetProxy(animationName)));
	}

	bool AnimationHelper::isAnimationPlaying() const
	{
		const qsf::MeshAnimationComponent* meshAnimationComponent = mEntity.getComponent<qsf::MeshAnimationComponent>();
		return (nullptr != meshAnimationComponent && meshAnimationComponent->isPlaying());
	}

	qsf::MeshAnimationChannel* AnimationHelper::playAnimation(const std::string& animationName, bool loop, bool reverse, qsf::Time blendTime)
	{
		return playAnimation(qsf::AssetProxy(animationName), loop, reverse, blendTime);
	}

	qsf::MeshAnimationChannel* AnimationHelper::playAnimation(const qsf::AssetProxy& animationAsset, bool loop, bool reverse, qsf::Time blendTime)
	{
		qsf::MeshAnimationComponent& meshAnimationComponent = mEntity.getOrCreateComponentSafe<qsf::MeshAnimationComponent>();
		if (blendTime <= qsf::Time::ZERO)
		{
			return meshAnimationComponent.playAnimation(animationAsset, loop, reverse);
		}
		else
		{
			return meshAnimationComponent.blendToAnimation(animationAsset, loop, blendTime, reverse);
		}
	}

	void AnimationHelper::clearAnimation()
	{
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			personComponent->clearAnimation();
		}
	}

	std::string AnimationHelper::getCurrentAnimation()
	{
		const qsf::MeshAnimationComponent* meshAnimationComponent = mEntity.getComponent<qsf::MeshAnimationComponent>();
		return (nullptr != meshAnimationComponent) ? meshAnimationComponent->getCurrentAnimation() : "";
	}

	std::string AnimationHelper::getCurrentAnimationDebugString()
	{
		const qsf::MeshAnimationComponent* meshAnimationComponent = mEntity.getComponent<qsf::MeshAnimationComponent>();
		return (nullptr != meshAnimationComponent) ? meshAnimationComponent->getDebugStringFirstChannel() : "";
	}

	std::string AnimationHelper::getWalkAnimation() const
	{
		const std::string animationName = "_walk_loop";
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			if (personComponent->getPersonFlags(PersonComponent::FLAG_DRUNK))
			{
				return ANIMATIONPATH_DEFAULT + "female_walk_drunk_loop"; //Todo: replace with gender-specific version
			}

#if 0	// Not necessary any more, getPersonType() does this already
			// Hard-coded disabling of this ugly senior walk animation, as requested by Martin D.
			if (personComponent->getAge() == PersonComponent::AGE_SENIOR)
			{
				return ANIMATIONPATH_PERSON + "male" + animationName;
			}
#endif
		}
		return ANIMATIONPATH_PERSON + getPersonType() + animationName;
	}

	std::string AnimationHelper::getRunAnimationBegin() const
	{
		const std::string animationName = "_run_start";
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent && personComponent->getPersonFlags(PersonComponent::FLAG_DRUNK))
		{
			return ANIMATIONPATH_DEFAULT + "female_run_drunk_begin"; //Todo: replace with gender-specific version
		}
		return ANIMATIONPATH_PERSON + getPersonType() + animationName;
	}

	std::string AnimationHelper::getRunAnimation(AnimationMaskData& animationMaskData) const
	{
		std::string animationName = "_walk_loop";
		EntityHelper entityHelper(mEntity);

		// Movement with equipment
		qsf::game::EquipmentComponent* activeEquipmentComponent = entityHelper.getActiveEquipmentComponent();
		if (nullptr != activeEquipmentComponent)
		{
			const qsf::AssetProxy& runAnimation = activeEquipmentComponent->getRunAnimation(); // TODO(mk) Using the run animation here is right. Currently we have idle and run animation, in future we support only one generic animation for both
			if (runAnimation.isValid())
			{
				if (activeEquipmentComponent->getUseSharedAnimation())
				{
					// Equipments can save only one type of name, we have to find the right animation
					animationMaskData.mMaskAnimation = qsf::AssetProxy(getRealPersonTypeOrShardedAnimationName(runAnimation.getLocalAssetName()));
					animationMaskData.mLoopAnimation = true;
				}
				else
				{
					return runAnimation;
				}
			}
		}

		// TODO(fw): This is experimental
#if 1

		bool isPanicking = false;
		bool isSearching = false;
		qsf::ActionComponent* actionComponent = mEntity.getComponent<qsf::ActionComponent>();
		// Check if move action hast movement mode "MOVEMENT_MODE_RUN_PANIC" -> panic move animation
		if (nullptr != actionComponent)
		{
			qsf::Action* currentAction = actionComponent->getCurrentAction();
			if (nullptr != currentAction && currentAction->getTypeId() == MoveAction::ACTION_ID)
			{
				em5::MoveAction* moveAction = static_cast<MoveAction*>(currentAction);
				isPanicking = moveAction->getMovementModeId() == MovementModes::MOVEMENT_MODE_RUN_PANIC;
				isSearching = moveAction->getMovementModeId() == MovementModes::MOVEMENT_MODE_SEARCHING;
			}
		}

		// Is person running
		qsf::ai::NavigationComponent* navigationComponent = mEntity.getComponent<qsf::ai::NavigationComponent>();
		if (nullptr != navigationComponent)
		{
			static const float minimumRunSpeedForRunAnimation = 2.01f;
			if (entityHelper.isSquadPerson() || navigationComponent->getMaxForwardSpeed() >= minimumRunSpeedForRunAnimation)
			{
				if (isPanicking)
				{
					// TODO(sw) use proper animation name for running in panic
					animationName = "_run_loop";
				}
				else
				{
					animationName = "_run_loop";
				}

				// HACK for martins animation fix
				CommandableComponent* commandableComponent = mEntity.getComponent<CommandableComponent>();
				if (nullptr != commandableComponent)
				{
					if (commandableComponent->isFireFighterPerson() || commandableComponent->isPolicePerson() ||
						commandableComponent->isPoliceSEKPerson() || commandableComponent->isTechEngineer())
					{
						return ANIMATIONPATH_PERSON + "female_run_loop";
					}
				}
			}
		}
#else
		qsf::MovableComponent* movableComponent = mEntity.getComponent<qsf::MovableComponent>();
		if (nullptr != movableComponent)
		{
			static const float minimumRunSpeedForRunAnimation = 0.6f;
			if (entityHelper.isSquadPerson() || movableComponent->getSpeed() >= minimumRunSpeedForRunAnimation)
			{
				animationName = "_run_loop";
			}
		}
#endif
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			// Check if person is drunk
			if (personComponent->getPersonFlags(PersonComponent::FLAG_DRUNK))
			{
				return ANIMATIONPATH_DEFAULT + "female_run_drunk"; //TODO(?) replace with gender-specific version
			}

			if (personComponent->getPersonFlags(PersonComponent::FLAG_NEW_SKELETON))
			{
				qsf::MetadataComponent* metadataComponent = mEntity.getComponent<qsf::MetadataComponent>();
				if (metadataComponent != nullptr && metadataComponent->getTags() == "cart_man")
				{
					// Here comes cartman!
					return ANIMATIONPATH_PERSON + "em5x_walk_cart";
				}

				if (personComponent->getGender() == PersonComponent::GENDER_MALE)
					return ANIMATIONPATH_PERSON + "em5x_sick_walk_2";
				else
					return ANIMATIONPATH_PERSON + "em5x_sick_walk_1";
			}

#if 0	// Not necessary any more, getPersonType() does this already
			// Hard-coded disabling of this ugly senior walk animation, as requested by Martin D.
			if (personComponent->getAge() == PersonComponent::AGE_SENIOR)
			{
				return ANIMATIONPATH_PERSON + "male" + animationName;
			}
#endif

			// Movement for animals
			if (personComponent->getGender() == PersonComponent::GENDER_ANIMAL)
			{
				if (isSearching)
				{
					return ANIMATIONPATH_ANIMAL + "rescue_dog_walk_search";
				}
				return ANIMATIONPATH_ANIMAL + "rescue_dog_run";
			}

			if (personComponent->getAge() == PersonComponent::AGE_CHILD /*|| personComponent->getAge() == PersonComponent::AGE_SENIOR*/)
			{
				// TODO(sw) HACK!!!! Due asset chaos of animation for children!!!!!
				if (animationName == "_run_loop")
				{
					return ANIMATIONPATH_PERSON + getPersonType() + animationName;
				}
				else
				{
					return ANIMATIONPATH_DEFAULT + getPersonType() + animationName;
				}
			}
		}

		return ANIMATIONPATH_PERSON + getPersonType() + animationName;
	}

	std::string AnimationHelper::getRunAnimationEnd() const
	{
		const std::string animationName = "_run_end";
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent && personComponent->getPersonFlags(PersonComponent::FLAG_DRUNK))
		{
			return ANIMATIONPATH_DEFAULT + "female_run_drunk_end"; //Todo: replace with gender-specific version
		}
		return ANIMATIONPATH_PERSON + getPersonType() + animationName;
	}

	std::string AnimationHelper::getSlowWalkAnimation() const
	{
		// TODO(mk) Implement this function
		return getWalkAnimation();
	}

	std::string AnimationHelper::getIdleAnimation(AnimationMaskData& animationMaskData) const
	{
		// In case of the person is injured, an special action updates the "idle" animation for them
		// TODO(mk) Check the position of the person (standing/laying/sitting)
		qsf::game::EquipmentComponent* activeEquipmentComponent = EntityHelper(mEntity).getActiveEquipmentComponent();
		if (nullptr != activeEquipmentComponent)
		{
			const qsf::AssetProxy& equipmentAnimation = activeEquipmentComponent->getIdleAnimation();	// TODO(mk) Using the idle animation here is right. Currently we have idle and run animation, in future we support only one generic animation for both cases
			if (equipmentAnimation.isValid())
			{
				if (activeEquipmentComponent->getUseSharedAnimation())
				{
					// Equipments can save only one type of name, we have to find the right animation
					animationMaskData.mMaskAnimation = qsf::AssetProxy(getRealPersonTypeOrShardedAnimationName(equipmentAnimation.getLocalAssetName()));
					animationMaskData.mLoopAnimation = true;
				}
				else
				{
					return equipmentAnimation;
				}
			}
			else
			{
				QSF_ASSERT(false, "em5::AnimationHelper::getIdleAnimation: The equipment \"" << activeEquipmentComponent->getEquipmentName() << "\" has no idle animation, is this intended?", QSF_REACT_NONE);
			}
		}

		// Handle animals
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			if (personComponent->getGender() == PersonComponent::GENDER_ANIMAL)
				return ANIMATIONPATH_ANIMAL + "rescue_dog_idle_01";

			if (personComponent->getPersonFlags(PersonComponent::FLAG_DRUNK))
				return ANIMATIONPATH_PERSON + "female_idle_drunk";

			if (personComponent->getPersonFlags(PersonComponent::FLAG_NEW_SKELETON))
			{
				qsf::MetadataComponent* metadataComponent = mEntity.getComponent<qsf::MetadataComponent>();
				if (metadataComponent != nullptr && metadataComponent->getTags() == "cart_man")
				{
					// Here comes cartman!
					return ANIMATIONPATH_PERSON + "em5x_idle_cart";
				}
				// TODO(fw): This one does not seem to exist, so I replaced it
				//return ANIMATIONPATH_PERSON + "em5x_sick_idle_stand";
				return ANIMATIONPATH_PERSON + "em5x_sick_2_stand";
			}
		}

		return ANIMATIONPATH_PERSON + getPersonType() + "_stand_idle_01";
	}

	std::string AnimationHelper::getAnimationLookingUp() const
	{
		return ANIMATIONPATH_PERSON + getPersonType() + "_looking_up";
	}

	std::string AnimationHelper::getAnimationBoardAnimation() const
	{
		std::string personType = getPersonType();
		if (personType == "animal")
		{
			return ANIMATIONPATH_ANIMAL + "rescue_dog_board";
		}

		// Use shared animation for male/female/child here
		return ANIMATIONPATH_PERSON + "board_vehicle";
	}

	std::string AnimationHelper::getAnimationDeboardAnimation() const
	{
		std::string personType = getPersonType();
		if (personType == "animal")
		{
			return ANIMATIONPATH_ANIMAL + "rescue_dog_deboard";
		}

		// Use shared animation for male/female/child here
		return ANIMATIONPATH_PERSON + "deboard_vehicle";
	}

	std::string AnimationHelper::getAnimationPlacePerson() const
	{
		// TODO(mk) Add check for gender
		return ANIMATIONPATH_PERSON + "board_vehicle_01";
	}

	std::string AnimationHelper::getAnimationFirefighterInstallHydrant(bool useFloorHydrant) const
	{
		if (useFloorHydrant)
		{
			return ANIMATIONPATH_DEFAULT + "female_manipulate_object_kneeling";
		}
		return ANIMATIONPATH_PERSON + "fireman_hose_install_hydrant";
	}

	std::string AnimationHelper::getAnimationFirefighterUseFireHose() const
	{
		return ANIMATIONPATH_PERSON + "fireman_hose_extinguish_loop";
	}

	std::string AnimationHelper::getAnimationFirefighterUseExtinguisher() const
	{
		return ANIMATIONPATH_PERSON + "fireman_fireextinguisher_extinguish_loop";
	}

	std::string AnimationHelper::getAnimationFirefighterUseRescueCutter1() const
	{
		return ANIMATIONPATH_PERSON + "fireman_scissors_cutting_01";
	}

	std::string AnimationHelper::getAnimationFirefighterUseRescueCutter2() const
	{
		return ANIMATIONPATH_PERSON + "fireman_scissors_cutting_02";
	}

	std::string AnimationHelper::getAnimationFirefighterUseRescueCutter3() const
	{
		return ANIMATIONPATH_PERSON + "fireman_scissors_cutting_03";
	}

	std::string AnimationHelper::getAnimationFirefighterUseAxe1() const
	{
		return ANIMATIONPATH_PERSON + "fireman_axe_open_door_01_start";
	}

	std::string AnimationHelper::getAnimationFirefighterUseAxe2() const
	{
		return ANIMATIONPATH_PERSON + "fireman_axe_open_door_01_loop";
	}

	std::string AnimationHelper::getAnimationFirefighterUseAxe3() const
	{
		return ANIMATIONPATH_PERSON + "fireman_axe_open_door_01_end";
	}

	std::string AnimationHelper::getAnimationFirefighterUseSaw1() const
	{
		return ANIMATIONPATH_PERSON + "fireman_saw_cutting_01";
	}

	std::string AnimationHelper::getAnimationFirefighterUseSaw2() const
	{
		return ANIMATIONPATH_PERSON + "fireman_saw_cutting_02";
	}

	std::string AnimationHelper::getAnimationFirefighterUseSaw3() const
	{
		return ANIMATIONPATH_PERSON + "fireman_saw_cutting_03";
	}

	std::string AnimationHelper::getAnimationFiremanPullStart() const
	{
		return ANIMATIONPATH_PERSON + "fireman_rescue_02";
	}

	std::string AnimationHelper::getAnimationFiremanPullLoop() const
	{
		return ANIMATIONPATH_PERSON + "fireman_rescue_03_loop";
	}

	std::string AnimationHelper::getAnimationFiremanPullEnd() const
	{
		return ANIMATIONPATH_PERSON + "fireman_rescue_03_end";
	}

	std::string AnimationHelper::getAnimationFiremanPullIdleStart() const
	{
		return ANIMATIONPATH_PERSON + "fireman_rescue_idle_start";
	}

	std::string AnimationHelper::getAnimationFiremanPullIdleLoop() const
	{
		return ANIMATIONPATH_PERSON + "fireman_rescue_idle_loop";
	}

	std::string AnimationHelper::getAnimationFiremanPullObjectStart() const
	{
		return ANIMATIONPATH_PERSON + "fireman_pull_object_01";
	}

	std::string AnimationHelper::getAnimationFiremanPullObjectLoop() const
	{
		return ANIMATIONPATH_PERSON + "fireman_pull_object_02";
	}

	std::string AnimationHelper::getAnimationFiremanPullObjectEnd() const
	{
		return ANIMATIONPATH_PERSON + "fireman_pull_object_03";
	}

	std::string AnimationHelper::getAnimationFiremanPutOnAbcSuit01() const
	{
		return ANIMATIONPATH_PERSON + "fireman_put_on_abcsuit_01";
	}

	std::string AnimationHelper::getAnimationFiremanPutOnAbcSuit02() const
	{
		return ANIMATIONPATH_PERSON + "fireman_put_on_abcsuit_02";
	}

	std::string AnimationHelper::getAnimationFiremanPutOnMask() const
	{
		return ANIMATIONPATH_PERSON + "fireman_put_on_mask";
	}

	std::string AnimationHelper::getAnimationFiremanLeadContaminatedLoop() const
	{
		return ANIMATIONPATH_PERSON + "fireman_lead_contaminated_loop";
	}

	std::string AnimationHelper::getAnimationFiremanIdleContaminatedLoop() const
	{
		return ANIMATIONPATH_PERSON + "fireman_idle_contaminated_loop";
	}

	std::string AnimationHelper::getAnimationDiverWalking() const
	{
		return ANIMATIONPATH_PERSON + "diver_walk_loop";
	}

	std::string AnimationHelper::getAnimationDiverStartSwimming() const
	{
		// TODO(mk) Because the enterwater animation is broken, use an other as work around
		//return ANIMATIONPATH_DEFAULT + "diver_swim_01";
		return ANIMATIONPATH_DEFAULT + "diver_swim_02";
	}

	std::string AnimationHelper::getAnimationDiverSwimming() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_swim_02";
	}

	std::string AnimationHelper::getAnimationDiverIdleWater() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_idle_water";
	}

	std::string AnimationHelper::getAnimationDiverIdleLand() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_idle_ground";
	}

	std::string AnimationHelper::getAnimationDiveDown() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_diving_01";
	}

	std::string AnimationHelper::getAnimationDiveUp() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_diving_02";
	}

	std::string AnimationHelper::getAnimationGrabDrowning() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_rescue_drowning_01";
	}

	std::string AnimationHelper::getAnimationRescueDrowningWater() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_rescue_drowning_02";
	}

	std::string AnimationHelper::getAnimationDiverDrowningLand() const
	{
		return ANIMATIONPATH_DIVER + "diver_drowning_ground";
	}

	std::string AnimationHelper::getAnimationRescueDrowningLand() const
	{
		return ANIMATIONPATH_DIVER + "diver_rescue_drowning_ground";
	}

	std::string AnimationHelper::getAnimationPutDrowningInBoat() const
	{
		return ANIMATIONPATH_DIVER + "diver_put_drowning_in_motorboat";
	}

	std::string AnimationHelper::getAnimationPutDrowningOnGround() const
	{
		return ANIMATIONPATH_DIVER + "diver_put_down_drowning_ground";
	}

	std::string AnimationHelper::getAnimationDiverEnterBoat() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_enter_motorboat";
	}

	std::string AnimationHelper::getAnimationDiverLeavingBoat() const
	{
		return ANIMATIONPATH_DEFAULT + "diver_leaving_motorboat_water";
	}

	std::string AnimationHelper::getAnimationParamedicRunFront() const
	{
		return ANIMATIONPATH_PERSON + "medic_front_walks_with_stretcher_01_loop";
	}

	std::string AnimationHelper::getAnimationParamedicRunBack() const
	{
		return ANIMATIONPATH_PERSON + "medic_back_walks_with_stretcher_02_loop";
	}

	std::string AnimationHelper::getAnimationParamedicIdleFront() const
	{
		return ANIMATIONPATH_PERSON + "medic_front_idle_with_stretcher";
	}

	std::string AnimationHelper::getAnimationParamedicIdleBack() const
	{
		return ANIMATIONPATH_PERSON + "medic_back_idle_with_stretcher";
	}

	std::string AnimationHelper::getAnimationParamedicLowerStretcherFront() const
	{
		return ANIMATIONPATH_PERSON + "medic_front_lower_stretcher";
	}

	std::string AnimationHelper::getAnimationParamedicLowerStretcherBack() const
	{
		return ANIMATIONPATH_PERSON + "medic_back_lower_stretcher";
	}

	std::string AnimationHelper::getAnimationParamedicRaiseStretcherFront() const
	{
		return ANIMATIONPATH_PERSON + "medic_front_raise_stretcher";
	}

	std::string AnimationHelper::getAnimationParamedicRaiseStretcherBack() const
	{
		return ANIMATIONPATH_PERSON + "medic_back_raise_stretcher";
	}

	std::string AnimationHelper::getAnimationDoctorGetEquipmentFromCar() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_get_bag_from_car";
	}

	std::string AnimationHelper::getAnimationEquipmentStretcherLower() const
	{
		return ANIMATIONPATH_EQUIPMENT + "equipment_medic_stretcher_lower";
	}

	std::string AnimationHelper::getAnimationEquipmentStretcherRaise() const
	{
		return ANIMATIONPATH_EQUIPMENT + "equipment_medic_stretcher_raise";
	}

	std::string AnimationHelper::getAnimationEquipmentOpenMedikit() const
	{
		return ANIMATIONPATH_EQUIPMENT + "equipment_emergency_medic_case_open_open";
	}

	std::string AnimationHelper::getAnimationEquipmentCloseMedikit() const
	{
		return ANIMATIONPATH_EQUIPMENT + "equipment_emergency_medic_case_open_close";
	}

	std::string AnimationHelper::getAnimationEquipmentOpenToolbox() const
	{
		return ANIMATIONPATH_EQUIPMENT + "equipment_engineer_toolbox_open_open_close";
	}

	std::string AnimationHelper::getAnimationEquipmentCloseToolbox() const
	{
		return getAnimationEquipmentCloseMedikit(); // Will likely have no own animation
	}

	std::string AnimationHelper::getAnimationVictimOnStretcher() const
	{
		// Shared animation
		return ANIMATIONPATH_INJURY + "victim_on_stretcher";
	}

	std::string AnimationHelper::getAnimationRescueDogFoundVictim() const
	{
		static std::string animationPool[2] = { "rescue_dog_found_victim_01", "rescue_dog_found_victim_02" };
		const std::string& randomAnimation = animationPool[qsf::Random::getRandomUint(0, 1)];
		return ANIMATIONPATH_ANIMAL + randomAnimation;
	}

	std::string AnimationHelper::getAnimationRescueDogSittingBark() const
	{
		return ANIMATIONPATH_ANIMAL + "rescue_dog_sitting_bark";
	}

	std::string AnimationHelper::getAnimationRescueDogArrivedLeader() const
	{
		return ANIMATIONPATH_ANIMAL + "rescue_dog_leader_call_dog";
	}

	std::string AnimationHelper::getAnimationPoliceDogMenace() const
	{
		return ANIMATIONPATH_ANIMAL + "rescue_dog_menace";
	}

	std::string AnimationHelper::getAnimationResueDogDie() const
	{
		return ANIMATIONPATH_ANIMAL + "rescue_dog_fall_down";
	}

	std::string AnimationHelper::getAnimationRescueDogLeaderSendDog() const
	{
		return ANIMATIONPATH_PERSON + "leader_rescue_dog_send_dog";
	}

	std::string AnimationHelper::getAnimationRescueDogLeaderCancelDog() const
	{
		return ANIMATIONPATH_PERSON + "leader_rescue_dog_call_dog";
	}

	std::string AnimationHelper::getAnimationDoctorHistoricSendPersonsHome() const
	{
		return ANIMATIONPATH_PERSON + "em5x_waggle";
	}

	std::string AnimationHelper::getAnimationDoctorHistoricCleanHome() const
	{
		return ANIMATIONPATH_PERSON + "em5x_mark_x";
	}

	std::string AnimationHelper::getAnimationPoliceArrestGangster01() const
	{
		return ANIMATIONPATH_PERSON + "policeman_arrest_01";
	}

	std::string AnimationHelper::getAnimationPoliceArrestGangster03() const
	{
		return ANIMATIONPATH_PERSON + "policeman_arrest_03";
	}

	std::string AnimationHelper::getAnimationPoliceLeadArrestedLoop() const
	{
		return ANIMATIONPATH_PERSON + "policeman_lead_arrested_gangster_walking_loop";
	}

	std::string AnimationHelper::getAnimationPoliceLeadArrestedIdle() const
	{
		return ANIMATIONPATH_PERSON + "policeman_lead_arrested_gangster_idle";
	}

	std::string AnimationHelper::getAnimationPoliceMegaphoneWalkingLoop() const
	{
		return ANIMATIONPATH_DEFAULT + "policeman_megaphone_walking_loop";
	}

	std::string AnimationHelper::getAnimationPoliceMegaphoneIdleLoop() const
	{
		return ANIMATIONPATH_DEFAULT + "policeman_megaphone_standing_loop";
	}

	std::string AnimationHelper::getAnimationPoliceBarrierTape01() const
	{
		return ANIMATIONPATH_PERSON + "policeman_barriertape_01";
	}

	std::string AnimationHelper::getAnimationPoliceBarrierTapeLoop() const
	{
		return ANIMATIONPATH_PERSON + "policeman_barriertape_02_loop";
	}

	std::string AnimationHelper::getAnimationPoliceBarrierTape03() const
	{
		return ANIMATIONPATH_PERSON + "policeman_barriertape_03";
	}

	std::string AnimationHelper::getAnimationPoliceStopPerson() const
	{
		return ANIMATIONPATH_PERSON + "policeman_stop_people_02_loop";
	}

	std::string AnimationHelper::getAnimationPolicePeppersprayStandingLoop() const
	{
		return ANIMATIONPATH_DEFAULT + "policeman_pepperspray_standing_01_loop";
	}

	std::string AnimationHelper::getAnimationPoliceRegulateTrafficLoop() const
	{
		return ANIMATIONPATH_DEFAULT + "policeman_regulate_traffic_02_loop";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistIdle() const
	{
		const int randomNumber = qsf::Random::getRandomInt(0, 5);
		if (randomNumber == 0)
			return ANIMATIONPATH_PERSON + "sekspecialist_idle";
		return ANIMATIONPATH_PERSON + "male_stand_idle_01";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistGetPistol() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_get_pistol";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistPutAwayPistol() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_put_away_pistol";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistIdlePistol() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_idle_pistol";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistPistol01() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_pistol_01";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistPistol02() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_pistol_02_loop";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistPistol02End() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_pistol_02_end";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistPistolShoot() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_pistol_shoot";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunPistol01() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_pistol_01";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunPistol02Loop() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_pistol_02_loop";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunPistol02End() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_pistol_02_end";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunShootPistol01() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_shoot_pistol_01";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunShootPistol02Loop() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_shoot_pistol_02_loop";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunShootPistol02End() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_shoot_pistol_02_end";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistLeadArrestedLoop() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_lead_arrested_02_loop";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistLeadArrestedIdle() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_lead_arrested_idle";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRopingLoop() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_roping_02";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRopingEnd() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_roping_03";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistGetRifle() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_get_rifle";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistPutAwayRifle() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_put_away_rifle";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistIdleRifle() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_idle_rifle";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistKneelRifleStart() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_kneel_rifle_start";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistKneelRifleLoop() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_kneel_rifle_loop";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistKneelRifleEnd() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_kneel_rifle_end";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistShootRifle() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_shoot_rifle";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunRifle01() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_rifle_01";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunRifle02Loop() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_rifle_02_loop";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistRunRifle02End() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_run_rifle_02_end";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistGetGrenade() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_get_grenade";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistIdleGrenade() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_idle_grenade";
	}

	std::string AnimationHelper::getAnimationSEKSpecialistThrowGrenade() const
	{
		return ANIMATIONPATH_PERSON + "sekspecialist_throw_grenade";
	}

	std::string AnimationHelper::getAnimationGuardsmanEquipSword() const
	{
		return ANIMATIONPATH_PERSON + "guardsmen_equip_sword";
	}

	std::string AnimationHelper::getAnimationSEKRobotDefuseBomb() const
	{
		return ANIMATIONPATH_DEFAULT + "em5_vs_sekbot_defuse";
	}

	std::string AnimationHelper::getAnimationRescueWithHeli01() const
	{
		if (getPersonType() == "senior")
		{
			// Extra for senior, use not shared animation
			return ANIMATIONPATH_PERSON + "senior_rescued_with_heli_01";
		}

		// Shared animation
		return ANIMATIONPATH_PERSON + "rescued_with_heli_01";
	}

	std::string AnimationHelper::getAnimationRescueWithHeli02() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "rescued_with_heli_02";
	}

	std::string AnimationHelper::getAnimationEngineerGetEquipmentFromCar() const
	{
		// Will likely get no own animation
		return getAnimationDoctorGetEquipmentFromCar();
	}

	std::string AnimationHelper::getAnimationEngineerOpenToolbox() const
	{
		// Will likely get no own animation
		return getAnimationDoctorOpenMedicCase();
	}

	std::string AnimationHelper::getAnimationEngineerGetDown() const
	{
		// Will likely get no own animation
		return getAnimationDoctorGetDown();
	}

	std::string AnimationHelper::getAnimationEngineerRepairSittingStart() const
	{
		return ANIMATIONPATH_PERSON + "female_manipulate_object_kneeling_01_start";
	}

	std::string AnimationHelper::getAnimationEngineerRepairSittingLoop() const
	{
		return ANIMATIONPATH_PERSON + "female_manipulate_object_kneeling_02_loop";
	}

	std::string AnimationHelper::getAnimationEngineerRepairSittingEnd() const
	{
		return ANIMATIONPATH_PERSON + "female_manipulate_object_kneeling_02_end";
	}

	std::string AnimationHelper::getAnimationEngineerRepairStandingStart() const
	{
		return ANIMATIONPATH_PERSON + "female_manipulate_object_standing_01_start";
	}

	std::string AnimationHelper::getAnimationEngineerRepairStandingLoop() const
	{
		return ANIMATIONPATH_PERSON + "female_manipulate_object_standing_02_loop";
	}

	std::string AnimationHelper::getAnimationEngineerRepairStandingEnd() const
	{
		return ANIMATIONPATH_PERSON + "female_manipulate_object_standing_02_end";
	}

	std::string AnimationHelper::getAnimationGangsterGettingArrested01() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "getting_arrested_01";
	}

	std::string AnimationHelper::getAnimationGangsterGettingArrested03() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "getting_arrested_03";
	}

	std::string AnimationHelper::getAnimationGangsterWalkArrestedLoop() const
	{
		return ANIMATIONPATH_PERSON + "walk_arrested_loop";
	}

	std::string AnimationHelper::getAnimationGangsterIdleArrested() const
	{
		return ANIMATIONPATH_PERSON + "idle_arrested";
	}

	std::string AnimationHelper::getAnimationGangsterCowerFearfullyStart() const
	{
		return ANIMATIONPATH_PERSON + getPersonType() + "_cower_fearfully_start";
	}

	std::string AnimationHelper::getAnimationGangsterCowerFearfullyLoop() const
	{
		return ANIMATIONPATH_PERSON + getPersonType() + "_cower_fearfully_loop";
	}

	std::string AnimationHelper::getAnimationGangsterCowerFearfullyEnd() const
	{
		return ANIMATIONPATH_PERSON + getPersonType() + "_cower_fearfully_end";
	}

	std::string AnimationHelper::getAnimationCivilGape() const
	{
		std::string animationVariant;
		if (getPersonType() == "male")
		{
			const int randomNumber = qsf::Random::getRandomInt(1, 3);
			switch (randomNumber)
			{
				case 1: animationVariant = "_gape_01"; break;
				case 2: animationVariant = "_gape_02"; break;
				// For extra variation, use the female gape animation for male
				case 3: return ANIMATIONPATH_DEFAULT + "female_gape_02";
			}
			return ANIMATIONPATH_DEFAULT + getPersonType() + animationVariant;
		}

		const int randomNumber = qsf::Random::getRandomInt(1, 2);
		switch (randomNumber)
		{
			case 1: animationVariant = "_gape_01"; break;
			case 2: animationVariant = "_gape_02"; break;
			// TODO(mk) Don't use "_gape_03" because it an gesture, making pictures with an mobile phone
			//case 3: animationVariant = "_gape_03"; break;
		}
		return ANIMATIONPATH_DEFAULT + getPersonType() + animationVariant;
	}

	std::string AnimationHelper::getAnimationCivilGape3() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_gape_03";
	}

	std::string AnimationHelper::getAnimationMobilephoneGetOutOfPocket() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_phone_mobile_phone_01_start";
	}

	std::string AnimationHelper::getAnimationMobilephonePutInPocket() const
	{
		// TODO(mk) We have only animations for male
		static const std::string animationPool[2] = { "_phone_mobile_phone_01_end", "_phone_mobile_phone_02_end"};
		return ANIMATIONPATH_DEFAULT + "male" + animationPool[qsf::Random::getRandomUint(0, 1)];
	}

	std::string AnimationHelper::getAnimationMobilephoneUseTalk() const
	{
		std::string animationName = "_phone_mobile_phone_01_loop";
		if (getPersonType() == "male" || getPersonType() == "senior")
		{
			static const std::string animationPool[2] = { "_phone_mobile_phone_01_loop", "_phone_mobile_phone_02_loop"};
			animationName = animationPool[qsf::Random::getRandomUint(0, 1)];
		}
		return ANIMATIONPATH_DEFAULT + getPersonType() + animationName;
	}

	std::string AnimationHelper::getAnimationMobilephoneUsePhotographNear() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_photograph_mobile_phone_01";
	}

	std::string AnimationHelper::getAnimationMobilephoneUsePhotograph() const
	{
		std::string animationName = "_photograph_mobile_phone_01";
		if (getPersonType() == "male")
		{
			static const std::string animationPool[2] = { "_photograph_mobile_phone_01", "_photograph_mobile_phone_02"};
			animationName = animationPool[qsf::Random::getRandomUint(0, 1)];
		}
		return ANIMATIONPATH_DEFAULT + getPersonType() + animationName;
	}

	std::string AnimationHelper::getAnimationUmbrellaOpen() const
	{
		// This is a shared animation
		return ANIMATIONPATH_DEFAULT + "male_stand_opening_umbrella";
	}

	std::string AnimationHelper::getAnimationUseObject() const
	{
		std::string personType = getPersonType();
		if (personType == "male")
		{
			// Shared animation for male/female only
			personType = "female";
		}
		return ANIMATIONPATH_DEFAULT + personType + "_manipulate_object_standing";
	}

	std::string AnimationHelper::getAnimationBootleWalk() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_walk_bottle_loop";
	}

	std::string AnimationHelper::getAnimationAngry() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_angry_01";
	}

	std::string AnimationHelper::getAnimationWaveObject() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_wave_object";
	}

	std::string AnimationHelper::getAnimationStandIdleObjectRighthand() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_stand_idle_object_righthand";
	}

	std::string AnimationHelper::getAnimationStandIdleBottle() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_stand_idle_bottle";
	}

	std::string AnimationHelper::getAnimationFemaleStandIdleBottle() const
	{
		return ANIMATIONPATH_DEFAULT + "female_stand_idle_bottle";
	}

	std::string AnimationHelper::getAnimationHooliganCheeringWithFlag() const
	{
		return ANIMATIONPATH_DEFAULT + "ge03_hooligan_cheering_with_flag";
	}

	std::string AnimationHelper::getAnimationHooliganCheeringWithBottle() const
	{
		return ANIMATIONPATH_DEFAULT + "ge03_hooligan_cheering_with_bottle";
	}

	std::string AnimationHelper::getAnimationFemalePointingWithBottle() const
	{
		return ANIMATIONPATH_DEFAULT + "female_GE07_pointing_with_bottle";
	}

	std::string AnimationHelper::getAnimationWaveHelpStanding() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_wave_help_standing";
	}

	std::string AnimationHelper::getAnimationWaveHelpFromWindow() const
	{
		std::string personType = getPersonType();
		if (personType == "male")
		{
			// Shared animation for male/female only
			personType = "female";
		}
		return ANIMATIONPATH_DEFAULT + personType + "_wave_help_from_window";
	}

	std::string AnimationHelper::getAnimationLookingUpWaving() const
	{
		std::string personType = getPersonType();
		return ANIMATIONPATH_PERSON + personType + "_looking_up_waving";
	}

	std::string AnimationHelper::getAnimationCheer() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_cheer";
	}

	std::string AnimationHelper::getAnimationDuckDownStart() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_duck_down_start";
	}

	std::string AnimationHelper::getAnimationDuckDownIdle() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_duck_down_idle";
	}

	std::string AnimationHelper::getAnimationDuckDownEnd() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_duck_down_end";
	}

	std::string AnimationHelper::getAnimationWince() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_wince";
	}

	std::string AnimationHelper::getAnimationTalkExcited() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_talk_excited";
	}

	std::string AnimationHelper::getAnimationTalkStanding() const
	{
		switch (qsf::Random::getRandomUint(1, 2))
		{
			case 1: return getAnimationTalkStanding01();
		}

		return getAnimationTalkStanding02();
	}

	std::string AnimationHelper::getAnimationTalkStanding01() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_talk_standing_01";
	}

	std::string AnimationHelper::getAnimationTalkStanding02() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_talk_standing_02";
	}

	std::string AnimationHelper::getAnimationPlayGuitar() const
	{
		// No male version available
		return ANIMATIONPATH_DEFAULT + "female_play_guitar";
	}

	std::string AnimationHelper::getAnimationManipulateObjectStanding() const
	{
		// TODO(mk) This is a shared animation, rename the corresponding json
		return ANIMATIONPATH_DEFAULT + "female_manipulate_object_standing";
	}

	std::string AnimationHelper::getAnimationManipulateObjectKneeling() const
	{
		// No male version available
		return ANIMATIONPATH_DEFAULT + "female_manipulate_object_kneeling";
	}

	std::string AnimationHelper::getAnimationBreatheDeeply() const
	{
		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			// In general, use "male" animations for all seniors, as requested by Martin D.
			if (personComponent->getPersonFlags(PersonComponent::FLAG_NEW_SKELETON))
			{
				return ANIMATIONPATH_DEFAULT + "em5x_male_cough";
			}
		}

		// No male version available
		return ANIMATIONPATH_DEFAULT + "female_breathe_deeply";
	}

	std::string AnimationHelper::getAnimationBreatheDeeplyLoop() const
	{
		// No male version available
		return ANIMATIONPATH_DEFAULT + "female_breathe_deeply_01_loop";
	}

	std::string AnimationHelper::getAnimationJumpDown() const
	{
		return ANIMATIONPATH_DEFAULT +getPersonType() + "_jump_down";
	}

	std::string AnimationHelper::getAnimationWitchCaptured() const
	{
		return ANIMATIONPATH_PERSON + "witch_captured";
	}

	std::string AnimationHelper::getAnimationWitchRescuing() const
	{
		return ANIMATIONPATH_PERSON + "guardsmen_witch_rescue";
	}

	std::string AnimationHelper::getCollapseAnimation(bool isKneeing) const
	{
		static const std::string animationPool[2] = { "generic_collaps_01", "generic_collaps_02"};
		static const std::string kneeingAnimation = "generic_collaps_03_start";
		const std::string& animationVariant = (isKneeing ? kneeingAnimation : animationPool[qsf::Random::getRandomUint(0, 1)]);

		return ANIMATIONPATH_PERSON + animationVariant;
	}

	std::string AnimationHelper::getAnimationGenericLightLying() const
	{
		return ANIMATIONPATH_INJURY + "light_generic_injured_lying";
	}

	std::string AnimationHelper::getAnimationGenericNormalLying() const
	{
		return ANIMATIONPATH_INJURY + "normal_generic_injured_lying";
	}

	std::string AnimationHelper::getAnimationGenericLightStanding() const
	{
		return ANIMATIONPATH_INJURY + "light_generic_injured_standing";
	}

	std::string AnimationHelper::getAnimationKneeingToLying() const
	{
		return ANIMATIONPATH_INJURY + "cowering_victim_lying_down";
	}

	std::string AnimationHelper::getAnimationKneeingToStanding() const
	{
		return ANIMATIONPATH_INJURY + "cowering_victim_standing_up";
	}

	std::string AnimationHelper::getAnimationInjuredDrowning() const
	{
		// Shared animation
		return ANIMATIONPATH_INJURY + "drowning_01";
	}

	std::string AnimationHelper::getAnimationInjuredDrowningDanger() const
	{
		// Shared animation
		return ANIMATIONPATH_INJURY + "drowning_02";
	}

	std::string AnimationHelper::getAnimationInjuredDrowningDead() const
	{
		// Shared animation
		return ANIMATIONPATH_INJURY + "drowning_03";
	}

	std::string AnimationHelper::getAnimationInjuredDead() const
	{
		static std::string animationPool[2] = { "death_01", "death_02" };
		const std::string& randomAnimation = animationPool[qsf::Random::getRandomUint(0, 1)];
		return ANIMATIONPATH_INJURY + randomAnimation;
	}

	std::string AnimationHelper::getAnimationInjuredPulledStart() const
	{
		return ANIMATIONPATH_INJURY + "light_generic_injured_rescue_lying_01";
	}

	std::string AnimationHelper::getAnimationInjuredPulledLoop() const
	{
		return ANIMATIONPATH_INJURY + "light_generic_injured_rescue_lying_03_loop";
	}

	std::string AnimationHelper::getAnimationInjuredPulledEnd() const
	{
		return ANIMATIONPATH_INJURY + "light_generic_injured_rescue_lying_03_end";
	}

	std::string AnimationHelper::getAnimationContaminatedWalkLoop() const
	{
		return ANIMATIONPATH_PERSON + "contaminated_walk_loop";
	}

	std::string AnimationHelper::getAnimationContaminatedIdleLoop() const
	{
		return ANIMATIONPATH_PERSON + "contaminated_idle_loop";
	}

	std::string AnimationHelper::getAnimationHangingHelpless() const
	{
		// TODO(mk) Move the animation in the correct category
		return ANIMATIONPATH_DEFAULT + "female_hanging_helplessly";
	}

	std::string AnimationHelper::getAnimationDoctorGetDown() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_put_down_bag_01";
	}

	std::string AnimationHelper::getAnimationDoctorOpenMedicCase() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_put_down_bag_02";
	}

	std::string AnimationHelper::getAnimationDoctorFromCaseToLyingPatient() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_change_instrument_02";
	}

	std::string AnimationHelper::getAnimationDoctorFromLyingPatientToCase() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_change_instrument_01";
	}

	std::string AnimationHelper::getAnimationDoctorStandUpWithInstruments() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_stand_up_with_instrument";
	}

	std::string AnimationHelper::getAnimationDoctorGetDefibrillator() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_get_defibrillator";
	}

	std::string AnimationHelper::getAnimationDoctorTurnFromBaseToMonitor() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_defibrillate_01_get_pads";
	}

	std::string AnimationHelper::getAnimationDoctorDefibrillatorGetPads() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_defibrillate_01";
	}

	std::string AnimationHelper::getAnimationDoctorDefibrillatorTurnToTarget() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_defibrillate_02_start";
	}

	std::string AnimationHelper::getAnimationDoctorDefibrillatorUse() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_defibrillate_02_loop";
	}

	std::string AnimationHelper::getAnimationDefibrillateTarget() const
	{
		return ANIMATIONPATH_INJURY + "defibrillate_victim";
	}

	std::string AnimationHelper::getAnimationDoctorDiagnosis() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_visual_inspection_victim_lying";
	}

	std::string AnimationHelper::getAnimationDoctorMedicalCare() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_touching_victim_lying";
	}

	std::string AnimationHelper::getAnimationDoctorPlagueQuickTest() const
	{
		return ANIMATIONPATH_DOCTOR + "doctor_medicine_victim_standing_01";
	}

	std::string AnimationHelper::getAnimationDrowningGetRescuedStart() const
	{
		return ANIMATIONPATH_DROWN + "drowning_gets_rescued_01";
	}

	std::string AnimationHelper::getAnimationDrowningGetRescuedMove() const
	{
		return ANIMATIONPATH_DROWN + "drowning_gets_rescued_02_loop";
	}

	std::string AnimationHelper::getAnimationDrowningGetsPutInBoat() const
	{
		return ANIMATIONPATH_DROWN + "drowning_gets_put_in_motorboat";
	}

	std::string AnimationHelper::getAnimationDrowningGround() const
	{
		return ANIMATIONPATH_DROWN + "drowning_ground_01";
	}

	std::string AnimationHelper::getAnimationDrowningGroundLoop() const
	{
		return ANIMATIONPATH_DROWN + "drowning_ground_02_loop";
	}

	std::string AnimationHelper::getAnimationDrowningGroundPlace() const
	{
		return ANIMATIONPATH_DROWN + "drowning_ground_02_end";
	}

	std::string AnimationHelper::getAnimationCraneExtend() const
	{
		return ANIMATIONPATH_DEFAULT + "em5_vs_wrecker_crane_jib_extend";
	}

	std::string AnimationHelper::getAnimationCraneLowToHigh() const
	{
		// TODO(tl): Only works for towcar
		return ANIMATIONPATH_DEFAULT + "em5_vs_wrecker_crane_base_low_to_high";
	}

	std::string AnimationHelper::getAnimationCraneLowToHigh_boat() const
	{
		// TODO(tl): Only works for transporter
		return ANIMATIONPATH_DEFAULT + "em5_vs_wrecker_crane_base_low-to-high";
	}

	std::string AnimationHelper::getAnimationHeavyCraneExtend() const
	{
		return ANIMATIONPATH_DEFAULT + "em5_vs_rescuetruck_crane_jib_extend";
	}

	std::string AnimationHelper::getAnimationHeavyCraneLowToHigh() const
	{
		return ANIMATIONPATH_DEFAULT + "em5_vs_rescuetruck_crane_base_up";
	}

	std::string AnimationHelper::getAnimationHeavyCraneCockpit() const
	{
		return ANIMATIONPATH_DEFAULT + "em5_vs_rescuetruck_crane_cockpit_arm_up";
	}

	std::string AnimationHelper::getAnimationHeavyCraneHook() const
	{
		return ANIMATIONPATH_DEFAULT + "em5_vs_rescuetruck_crane_hook_swing";
	}

	std::string AnimationHelper::getAnimationSupportLegExtend() const
	{
		if (nullptr != mEntity.getComponent<HelicopterComponent>())
		{
			return ANIMATIONPATH_VEHICLE + "em5_vs_sekhelicopter_fast_rope_extend";
		}

		if (UnitHelper(mEntity).isFireFighterDLK())
		{
			return ANIMATIONPATH_DEFAULT + "em5_vs_laddertruck_main_support_left_support";
		}

		VehicleCraneComponent* vehicleCraneComponent = mEntity.getComponent<VehicleCraneComponent>();
		if (nullptr != vehicleCraneComponent)
		{
			if (vehicleCraneComponent->getCraneType() == VehicleCraneComponent::HEAVY_CRANE)
			{
				return ANIMATIONPATH_DEFAULT + "em5_vs_rescuetruck_support_support_out";
			}
		}

		return ANIMATIONPATH_VEHICLE + "hydraulic_support_left_support";
	}

	std::string AnimationHelper::getAnimationHelicopterHatchOpen() const
	{
		return ANIMATIONPATH_VEHICLE + "em5_recoveryhelicopter_hatch_open";
	}

	std::string AnimationHelper::getAnimationHelicopterHatchClose() const
	{
		return ANIMATIONPATH_VEHICLE + "em5_recoveryhelicopter_hatch_close";
	}

	std::string AnimationHelper::getAnimationAirplaneHatchOpen() const
	{
		return ANIMATIONPATH_VEHICLE + "em5_vs_firefightingplane_hatch_open";
	}

	std::string AnimationHelper::getAnimationImtSidesOpen() const
	{
		return ANIMATIONPATH_VEHICLE + "imt_sides_open";
	}

	// Fight animations

	// Fistfight
	std::string AnimationHelper::getAnimationFistFight01Start() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_fist_01_start";
	}
	std::string AnimationHelper::getAnimationFistFight01Loop() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_fist_01_loop";
	}
	std::string AnimationHelper::getAnimationFistFight01End() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_fist_01_end";
	}
	std::string AnimationHelper::getAnimationFistFight02() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_fist_02";
	}

	// Baseball bat
	std::string AnimationHelper::getAnimationFightBaseballBat01Start() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_baseballbat_01_start";
	}
	std::string AnimationHelper::getAnimationFightBaseballBat01Loop() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_baseballbat_01_loop";
	}
	std::string AnimationHelper::getAnimationFightBaseballBat01End() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_baseballbat_01_end";
	}
	std::string AnimationHelper::getAnimationFightBaseballBat02() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_baseballbat_02";
	}

	// Knife
	std::string AnimationHelper::getAnimationFightKnife01Start() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_knife_01_start";
	}
	std::string AnimationHelper::getAnimationFightKnife01Loop() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_knife_01_loop";
	}
	std::string AnimationHelper::getAnimationFightKnife01End() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_knife_01_end";
	}
	std::string AnimationHelper::getAnimationFightKnife02() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_knife_02";
	}

	// Torch
	std::string AnimationHelper::getAnimationFightTorch01Start() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_torch_01_start";
	}
	std::string AnimationHelper::getAnimationFightTorch01Loop() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_torch_01_loop";
	}
	std::string AnimationHelper::getAnimationFightTorch01End() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_torch_01_end";
	}
	std::string AnimationHelper::getAnimationFightTorch02() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_torch_02";
	}

	// Sword
	std::string AnimationHelper::getAnimationFightSword01Start() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_sword_01_start";
	}
	std::string AnimationHelper::getAnimationFightSword01Loop() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_sword_01_loop";
	}
	std::string AnimationHelper::getAnimationFightSword01End() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_sword_01_end";
	}
	std::string AnimationHelper::getAnimationFightSword02() const
	{
		// Shared animation
		return ANIMATIONPATH_PERSON + "fight_sword_02";
	}

	std::string AnimationHelper::getAnimationGettingHit() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_getting_hit";
	}

	std::string AnimationHelper::getAnimationIdleThrowingWeapon() const
	{
		return ANIMATIONPATH_DEFAULT + getPersonType() + "_idle_throwing_weapon";
	}
	std::string AnimationHelper::getAnimationThrowObject() const
	{
		// TODO(mk) This is a shared animation, rename the corresponding json
		return ANIMATIONPATH_DEFAULT + "female_throw_object";
	}

	std::string AnimationHelper::getAnimationShootPistolStanding() const
	{
		return ANIMATIONPATH_PERSON + getPersonType() + "_shoot_pistol_standing";
	}

	std::string AnimationHelper::getAnimationShootPistolRunningStart() const
	{
		// TODO(mk) This is a shared animation, rename the corresponding json
		return ANIMATIONPATH_DEFAULT + "female_shoot_pistol_running_start";
	}

	std::string AnimationHelper::getAnimationShootPistolRunningLoop() const
	{
		// TODO(mk) This is a shared animation, rename the corresponding json
		return ANIMATIONPATH_DEFAULT + "female_shoot_pistol_running_loop";
	}

	std::string AnimationHelper::getAnimationShootPistolRunningEnd() const
	{
		// TODO(mk) This is a shared animation, rename the corresponding json
		return ANIMATIONPATH_DEFAULT + "female_shoot_pistol_running_end";
	}

	std::string AnimationHelper::getAnimationKneelingWithWeaponStart() const
	{
		return ANIMATIONPATH_PERSON + "generic_kneeling_with_pistol_start";
	}

	std::string AnimationHelper::getAnimationKneelingWithWeaponLoop() const
	{
		return ANIMATIONPATH_PERSON + "generic_kneeling_with_pistol_loop";
	}

	std::string AnimationHelper::getAnimationKneelingWithWeaponEnd() const
	{
		return ANIMATIONPATH_PERSON + "generic_kneeling_with_pistol_end";
	}

	std::string AnimationHelper::getAnimationStunnedByPepperspray() const
	{
		// TODO(sw) As soon as there is a "male" version of that animation, fix this hack:
		std::string personType = getPersonType();
		if ("male" == personType)
		{
			// Use female instead...
			personType = "female";
		}
		return ANIMATIONPATH_DEFAULT + personType + "_stunned_pepperspray";
	}

	std::string AnimationHelper::getAnimationStunnedByFlashGrenade() const
	{
		return ANIMATIONPATH_PERSON + getPersonType() + "_stunned_flashgrenade";
	}

	std::string AnimationHelper::getAnimationBuoy() const
	{
		return ANIMATIONPATH_DEFAULT + "equipment_firefighter_buoy_swim";
	}

	std::string AnimationHelper::getAnimationWellPumping() const
	{
		return ANIMATIONPATH_DEFAULT + "water_pump_pump";
	}

	std::string AnimationHelper::getAnimationSupervisorSpeaking() const
	{
		return ANIMATIONPATH_DEFAULT + "hu_m_special_001_supervisor_facil_with_arm_movement";
	}

	std::string AnimationHelper::getAnimationSupervisorIdle() const
	{
		// TODO(co) Someone moved the asset "em5/skeleton/default/hu_m_special_001_supervisor_idle" to "em5/skeleton/person/hu_m_special_001_supervisor_idle" resulting in T-pose supervisor
		// return ANIMATIONPATH_DEFAULT + "hu_m_special_001_supervisor_idle";
		return ANIMATIONPATH_PERSON + "hu_m_special_001_supervisor_idle";
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	const std::string& AnimationHelper::getPersonType() const
	{
		static const std::string maleType = "male";		// Fallback

		PersonComponent* personComponent = mEntity.getComponent<PersonComponent>();
		if (nullptr != personComponent)
		{
			// In general, use "male" animations for all seniors, as requested by Martin D.
			if (personComponent->getAge() == PersonComponent::AGE_SENIOR)
			{
				return maleType;
			}

			return personComponent->getPersonTypeAsString();
		}

		// Error
		return maleType;
	}

	std::string AnimationHelper::getRealPersonTypeOrShardedAnimationName(const std::string& animationName) const
	{
		// Get the person type from the animation name
		auto begin = animationName.find_last_of('/') +1;
		std::string pureAnimationPath = animationName.substr(begin, animationName.size() - begin);
		size_t endpos = pureAnimationPath.find_first_of('_');
		std::string personType = pureAnimationPath.substr(0, endpos);

		// Check if animation name matches person type
		std::string currentPersonType = getPersonType();
		if (personType != currentPersonType)
		{
			// Build real animation name
			const std::string realAnimationName = animationName.substr(0, begin) + currentPersonType + animationName.substr(begin+endpos, animationName.size() - endpos);

			// Check if "real" animation is existing
			if (nullptr != qsf::AssetProxy(realAnimationName).getCachedAsset())
			{
				return realAnimationName;
			}

			// Build shared animation name (currently we use the female animation as shared animation)
			std::string sharedAnimationName = animationName.substr(0, begin) + "female" + animationName.substr(endpos, animationName.size() - endpos);
			if (nullptr != qsf::AssetProxy(sharedAnimationName).getCachedAsset())
			{
				// Take shared animation as fallback
				return sharedAnimationName;
			}
		}

		// Animation is valid for the person type
		return animationName;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
