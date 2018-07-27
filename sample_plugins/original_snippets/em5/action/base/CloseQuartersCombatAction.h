// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"
#include "em5/component/equipment/weapon/Weapon.h"

#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/logic/action/Action.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class HealthComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 close quarters combat action
	*/
	class EM5_API_EXPORT CloseQuartersCombatAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::PullObjectAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CloseQuartersCombatAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CloseQuartersCombatAction();

		/**
		*  @brief
		*    Initialize action with values
		*
		*  @param[in] targetEntity
		*    The target of the action
		*  @param[in] attackerWeapon
		*    The attacker's weapon
		*  @param[in] targetWeapon
		*    The target's weapon
		*  @param[in] maxCombatRounds
		*    The maximum number of combat rounds the opponents fight, 0 for "fight to zero life"
		*  @param[in] lowerTargetLife
		*    Amount of life (relative to max. life) the target loses each round (leave uninitialized for default weapon damage)
		*  @param[in] lowerAttackerLife
		*    Amount of life (relative to max. life) the attacker loses each round (leave uninitialized for default weapon damage)
		*  @param[in] lifeRestrictionPercentage
		*    A limit how low the life can get in percentage (This is mainly used for the tutorial to avoid fight to injure but have an unlimited fight)
		*/
		void init(const qsf::Entity& targetEntity, weapon::Weapon attackerWeapon, weapon::Weapon targetWeapon, uint64 maxCombatRounds = 0, float lowerTargetLife = qsf::getUninitialized<float>(0.0f), float lowerAttackerLife = qsf::getUninitialized<float>(0.0f), float healthRestrictionPercentage = 0.0f);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum CombatState
		{
			COMBATSTATE_INIT,			///< The fight starts here.
			COMBATSTATE_ATTACK,			///< Attacker attacks target. In a fist fight, the fighting act happens in here only.
			COMBATSTATE_CHECK_INJURIES,	///< Check for injuries
			COMBATSTATE_COUNTERATTACK,	///< In case this is not a fist fight, there always is a counter attack from the target.
			COMBATSTATE_DONE			///< When the fight is done, this state is reached
		};

		// As used in "enqueueFightingAnimations()" - Needed to enqueue parts of
		enum FightAnimationMode
		{
			FULL_ANIMATION,	///< Play the full animation
			FIRST_PART,		///< Only play the part until the (last) hit. Weapon-specific.
			LAST_PART		///< Only play the part from the (last) hit to the end. Weapon-specific.
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Enqueue the animations for exactly one fighting round
		void enqueueFightingAnimations(qsf::Entity& attackerEntity, qsf::Entity& targetEntity, weapon::Weapon attackerWeapon, weapon::Weapon targetWeapon, FightAnimationMode animationMode = FULL_ANIMATION);

		// Returns a human readable string for the name of the given weapon
		std::string getDebugWeaponName(weapon::Weapon weapon) const;

		// Returns the default damage the specified weapon does each round
		float getDefaultWeaponDamage(weapon::Weapon weapon) const;

		// Return an injury (random!) that was caused by the given weapon
		qsf::StringHash getInjuryCausedByWeapon(weapon::Weapon weapon) const;

		bool damageEntity(HealthComponent& healthComponent, float damage, qsf::Entity& damgeSourceEntity);

		// Internal helper function for playing animation
		void playAnimationStartByWeapon(qsf::Entity& entity, weapon::Weapon weaponType);
		void playAnimationEndByWeapon(qsf::Entity& entity, weapon::Weapon weaponType);
		void playAnimationWithOffset(qsf::Entity& entity, std::string animationPfath, qsf::Time blendTime, float startOffset, float endOffset, const boost::optional<qsf::Time> maxPlayTime = boost::optional<qsf::Time>());


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64				mTargetEntityId;				///< Target entity ID
		CombatState			mState;							///< State of the action
		weapon::Weapon		mAttackerWeapon;				///< The weapon used by the owner
		weapon::Weapon		mTargetWeapon;					///< The weapon used by the target
		float				mLowerTargetLife;				///< How much of the target's life (relative to his max life) is lost each round?
		float				mLowerAttackerLife;				///< How much of the attacker's life (relative to his max life) is lost each round?
		uint64				mMaxCombatRounds;				///< How many rounds shall be fought? Convention: 0 means "fight until one opponent has no health left".
		uint64				mCurrentRound;					///< Keep track of the current combat round
		bool				mTargetInjured;					///< Is the target injured?
		bool				mAttackerInjured;				///< Is the attacker injured?
		bool				mCounterAttack;					///< Is it currently the second part of a round where the target does a counter-attack?
		float				mLowerHealthLimitPercentage;	///< The limit (in percent) how low the health can get in an endless fight (Needed for the tutorial)
		AudioProxy			mAudioProxy;					///< Audio proxy (needed to handle delay)
		AudioProxy			mKickAudioProxy;				///< Audio proxy (needed to handle delay)

		// Debug
		//  -> Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define
		qsf::DebugDrawProxy	mDebugDrawProxy;				///< Debug draw proxy for text output


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::CloseQuartersCombatAction)
