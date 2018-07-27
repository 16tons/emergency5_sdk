// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/damage/DamageType.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/health/injury/Injury.h"
#include "em5/health/injury/Injuries.h"

#include <qsf_game/component/health/HealthComponent.h>

#include <qsf/message/MessageProxy.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/job/JobProxy.h>
#include <qsf/math/BitFlagSet.h>
#include <qsf/math/EnumBitmask.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
	namespace multiplayer
	{
		class HealthData;
	}
}
namespace qsf
{
	class TransformComponent;
	class MeshComponent;
	class MessageParameters;
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
	*    EMERGENCY 5 health component class
	*
	*  @note
	*    - Overview over the types of energies:
	*       -> Health energy: The energy when the person is not injured
	*       -> Life energy:   The energy when the person is injured
	*       -> Death energy:  The energy when the person is injured and had lost all life energy
	*/
	class EM5_API_EXPORT HealthComponent : public qsf::game::HealthComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		// TODO(sw) This is only needed for multiplay and there only on client side
		//			It would be better, when the simulation and pure visually updates are split into separate methods
		//			The pure visually updates could then be called cyclic on the multiplayer client (via an update job) to update the visually aspects
		friend class multiplayer::HealthData;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;			///< "em5::HealthComponent" unique component ID
		static const uint32 HEALTH_ENERGY;			///< Current health energy
		static const uint32 MAX_HEALTH_ENERGY;		///< Maximum health energy
		static const uint32 LIFE_ENERGY;			///< Current life energy
		static const uint32 MAX_LIFE_ENERGY;		///< Maximum life energy
		static const uint32 DEATH_ENERGY;			///< Current death energy
		static const uint32 MAX_DEATH_ENERGY;		///< Maximum death energy

		/** Health state */
		// TODO(sw) When extending this enum with additional state also multiplayer::HealthData must be updated so that it writes the additional values (currently only 2 bits are written)
		enum State
		{
			STATE_HEALTHY,		///< Person is not injured
			STATE_INJURED,		///< Person is injured, possibly already treated
			STATE_DEATHFIGHT,	///< Person is near death, but can still be reanimated
			STATE_DEAD			///< Person is dead
		};

		/** Immunity type */
		enum ImmunityType
		{
			IMMUNITY_INJURY,					// Immunity to normal injuries
			IMMUNITY_ILLNESS,					// Immunity to illnesses
			IMMUNITY_CONTAMINATION_RADIOACTIVE,	// Immunity to radioactive contamination
			IMMUNITY_CONTAMINATION_CHEMICAL,	// Immunity to chemical contamination
			IMMUNITY_CONTAMINATION_PLAGUE,		// Immunity to plague contamination
			IMMUNITY_CONTAMINATION_BIOLOGICAL	// Immunity to biological contamination
		};
		typedef qsf::EnumBitmask<uint8, ImmunityType> ImmunityBitmask;

		// Shortcuts
		typedef Injury::InjuryFlags InjuryFlags;
		typedef Injury::InjuryBitmask InjuryBitmask;
		typedef Injury::ContaminationType ContaminationType;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit HealthComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~HealthComponent();

		/**
		*  brief
		*    Use this method to injure a person by an event
		*/
		bool injurePersonByEventById(uint32 injuryId, FreeplayEvent* freeplayEvent, bool playCollapseAnimation = true);

		void killPerson(FreeplayEvent* freeplayEvent, const VisualVitalSigns* visualVitalSigns = nullptr, const qsf::StringHash& InjuryId = injury::DUMMY_INJURY);

		//[-------------------------------------------------------]
		//[ Health energy                                         ]
		//[-------------------------------------------------------]
		bool isReceivingFireDamage() const;

		// Decrease the health energy of an person
		void receiveDamage(float amount, damagetype::DamageType damageType, qsf::Component* sender);

		// Return the health energy
		inline float getHealthEnergy() const;

		// Set the health energy
		void setHealthEnergy(float healthEnergy);

		// Return the maximum health energy
		inline float getMaximumHealthEnergy() const;

		// Set the maximum health energy
		void setMaximumHealthEnergy(float MaximumHealthEnergy);

		//[-------------------------------------------------------]
		//[ Life energy                                           ]
		//[-------------------------------------------------------]
		// Return the life energy, active in case the person is injured
		inline float getLifeEnergy() const;

		// Set the life energy, active in case the person is injured
		void setLifeEnergy(float lifeEnergy);

		// Return the maximum life energy
		inline float getMaximumLifeEnergy() const;

		// Set the maximum life energy
		void setMaximumLifeEnergy(float maximumLifeEnergy);

		//[-------------------------------------------------------]
		//[ Death energy                                          ]
		//[-------------------------------------------------------]
		// Return the death energy
		inline float getDeathEnergy() const;

		// Set the death energy
		void setDeathEnergy(float deathEnergy);

		// Return the maximum death energy
		inline float getMaximumDeathEnergy() const;

		// Set the maximum death energy
		void setMaximumDeathEnergy(float maximumDeathEnergy);

		//[-------------------------------------------------------]
		//[ State                                                 ]
		//[-------------------------------------------------------]
		// Return the current health state
		inline State getState() const;

		// Return "true" if the person is healthy (but may be injured)
		inline bool isHealthy() const;

		// Return "true" if the person is injured (but may be healthy)
		inline bool isInjured() const;

		// Return "true" if the person is dead
		inline bool isDead() const;

		// Return "true" if the entity is allowed to move
		inline bool isMovable() const;

		// Get the injury bitmask
		inline Injury::InjuryBitmask getInjuryBitmask() const;

		inline VisualVitalSigns& getVisualVitalSigns();

		/**
		*  @brief
		*    Return if the person is drowning
		*/
		inline bool getIsDrowning() const;

		/**
		*  @brief
		*    Return if the person is kneeing due injury
		*/
		inline bool getIsKneeing() const;

		// when put on land the person is no longer drowning
		void setDrowningFollowUpInjury();

		// Will be close to 0 as long as unit stands near the fire.
		const qsf::Time& getElapsedTimeAfterLastHeatEnergyReceived() const;

		//[-------------------------------------------------------]
		//[ Injury                                                ]
		//[-------------------------------------------------------]
		// Get injury
		inline const Injury* getInjury() const;

		// Get injury ID
		inline uint32 getInjuryId() const;

		// Apply an injury
		bool applyInjury(const Injury& injury, FreeplayEvent* freeplayEvent, bool playCollapseAnimation = true);

		// Apply an injury
		bool applyInjuryById(const qsf::StringHash& id, FreeplayEvent* freeplayEvent, bool playCollapseAnimation = true);

		// Remove the current injury and set the health state back to healthy
		void healAllInjuries();

		void doctorTreatmentFinished();

		void updateInjuredIcon(bool forceUpdate = false);

		inline bool getCanDoContaminationQuickTest() const;
		inline void setCanDoContaminationQuickTest(bool canDoContaminationQuickTest);

		//[-------------------------------------------------------]
		//[ Invincibility                                         ]
		//[-------------------------------------------------------]
		inline bool isInvincible() const;
		inline void setInvincible(bool invincible);

		//[-------------------------------------------------------]
		//[ Immunities                                            ]
		//[-------------------------------------------------------]
		inline const ImmunityBitmask& getImmunities() const;

		bool isImmune(ImmunityType immunityType) const;

		// Is the current entity immune to the given injury
		bool isImmune(const Injury& injury) const;

		void setImmunityType(ImmunityType immunityType, bool value = true);

		// Get the immunities as string
		std::string getImmunitiesAsString() const;

		// Set the immunities by string
		void setImmunitiesByString(const std::string& immunities);

		//[-------------------------------------------------------]
		//[ Resistances                                           ]
		//[-------------------------------------------------------]
		// Get the resistances as string
		std::string getResistancesAsString() const;

		// Set the resistances by string
		void setResistancesByString(const std::string& resistances);

		// Get the resistances array
		float* getResistancesArray();

		//[-------------------------------------------------------]
		//[ Regeneration                                          ]
		//[-------------------------------------------------------]
		// Check if regeneration in healthy state is disabled
		bool isRegenerationDisabled() const;

		// Disable or re-enable regeneration in healthy state
		void setRegenerationDisabled(bool noRegeneration);

		//[-------------------------------------------------------]
		//[ Treatment                                             ]
		//[-------------------------------------------------------]
		// Return "true" if this patient is currently treated by a doctor
		inline bool isTreatmentActive() const;

		// Return "true" if this patient is currently diagnosed by a doctor
		inline bool isDiagnosisActive() const;

		// Return "true" if the diagnosis is complete
		inline bool isDiagnosisComplete() const;

		// Get the ID of the current injury diagnosis
		inline uint32 getDiagnosisId() const;

		// Activate or deactivate treatment
		inline void setTreatmentActive(bool treatment);

		// Activate or deactivate diagnosis
		inline void setDiagnosisActive(bool diagnosis);

		// Activate or deactivate medical care
		inline void setMedicalCareActive(bool care);

		// Activate or deactivate reanimation
		inline void setReanimationActive(bool reanimantion);

		// Set status of the examination of the doctor
		void setDiagnosisComplete(bool examined);

		// Set all flags to have the target ready for transport
		// Not part of the original control flow, only for debug
		inline void setInjuredHealed();

		//[-------------------------------------------------------]
		//[ Contamination                                         ]
		//[-------------------------------------------------------]
		// Return "true" if the person is contaminated
		inline bool isContaminated() const;

		// Get contamination type
		inline ContaminationType getContaminationType() const;

		// Contaminate (used for re-contamination, otherwise the injury sets the contamination already)
		void contaminate(ContaminationType contaminationType);

		// Decontaminate
		void decontaminate();

		//[-------------------------------------------------------]
		//[ Cleanup                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Check whether this component is ready for cleanup
		*/
		inline bool isReadyForCleanup() const;

		/**
		*  @brief
		*    Perform the cleanup
		*/
		void cleanup();

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		// Return formatted string for debug display
		std::string getFormattedDebugText() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onSetSimulating(bool simulating) override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void delayedInfection(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		//[-------------------------------------------------------]
		//[ Lock                                                  ]
		//[-------------------------------------------------------]
		// Lock the actions of the entity
		void lockActions(bool playCollapseAnimation=true) const;

		// Unlock the actions of the entity
		void unlockActions() const;

		//[-------------------------------------------------------]
		//[ Update simulation                                     ]
		//[-------------------------------------------------------]
		// Job updates
		void updateSimulation(const qsf::JobArguments& jobArguments);

		// Manage the status of a healthy person
		void manageHealthyState(float secondsPassed);

		// Manage the status of an injured person
		void manageInjuredState(float secondsPassed);

		// Manage the status of a person in the death fight
		void manageDeathFightState(float secondsPassed);

		// Apply the fire damage to the health
		void applyFireDamage();

		void updateInfectionSpreading(const qsf::JobArguments& jobArguments);

		// Infect other person in infection radius
		void processInjurySpreading(const Injury& injury);

		// Perform a state change
		void changeState(State state);

		// Switch to the injury following the currently set injury (for illnesses and contaminations)
		void switchToFollowingInjury();

		// Process all visual vital signs that need to be set
		void processVisualVitalSign(const VisualVitalSigns& visualVitalSigns);

		/**
		*  @brief
		*    If the owner has an new injury, updated the health values from the injury
		*/
		void applyHealthValuesFromInjury(bool forceUpdate = false);

		void testAndTurnTargetFromObjectsBehind();

		void showHitpointBar();

		//[-------------------------------------------------------]
		//[ Hint                                                  ]
		//[-------------------------------------------------------]
		void showDamageHint(float oldHealthEnergy) const;
		void showHintReceiveAttackDamage() const;
		void showMiniMapIconReceiveFireDamage() const;

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		// Show debug output
		void showDebug();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			HEALTH_ACTIVE_TREATMENT		= 1<<0,		///< Set if a doctor currently cares for this patient (either diagnosis, medical care, reanimation, or a brief intermediate state)
			HEALTH_ACTIVE_DIAGNOSIS		= 1<<1,		///< Set if diagnosis mode is active, i.e. a doctor currently does a diagnosis on this patient
			HEALTH_ACTIVE_MEDICAL_CARE	= 1<<2,		///< Set if medical care is currently applied
			HEALTH_ACTIVE_REANIMATION	= 1<<3,		///< Set if a reanimation is currently applied
			HEALTH_DIAGNOSIS_COMPLETE	= 1<<4,		///< Set if the patient was already examined by a doctor (i.e. successful diagnosis), so that the health state is shown to the player now
			HEALTH_TREATMENT_COMPLETE	= 1<<5,		///< Set if the patient was healed to the injury threshold
			HEALTH_INJURY_HEALED		= 1<<6,		///< Set if the patient was healed to maximum life
													///< 1<<7 can be used again
			HEALTH_NO_REGENERATION		= 1<<8		///< Set if the person should not regenerate energy while healthy
		};
		typedef qsf::BitFlagSet<uint16, InternalFlags> InternalFlagSet;


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static qsf::MessageProxy	mInfectionDelayMessageProxy;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP system
		float				mHealthEnergy;				///< Health energy of the person
		float				mMaximumHealthEnergy;		///< Maximum health energy every person can receive, if health reaches zero the person is injured and lose life energy
		float				mLifeEnergy;				///< Life energy of the person
		float				mMaximumLifeEnergy;			///< Maximum life energy every person can receive, if life reaches zero the person starts his death fight
		float				mDeathEnergy;				///< Death energy of the person, not reflected to camp
		float				mMaximumDeathEnergy;		///< Maximum death energy every person can receive, in death fight state person loses continually energy, if reach zero the person dies, not reflected to camp

		// Internal
		State				mState;						///< Condition of the person
		InternalFlagSet		mInternalFlags;				///< Set of internal flags, see enum "InternalFlags"
		float				mEnergyChangeThisUpdate;	///< Track of amount of losing / getting (health/life/death) energy in this simulation update
		qsf::Time			mRegenerationBlockedTime;
		float				mFireDamageThisUpdate;		///< Backup from the fire damage (Can be from more than one source), is applied every update tick for healthy persons
		qsf::WeakPtr<qsf::Component> mDamageSenderComponent;
		StatusOverlayComponent::BarType mHealthBarShown;	///< Currently shown bar type (health, life or none)

		// Injury
		const Injury*		mInjury;					///< Active injury or illness, or a null pointer
		ContaminationType	mContamination;				///< Active contamination type
		VisualVitalSigns	mVisualVitalSigns;			///< Keeps information about the visual vital sign of the target
		bool				mInvincible;				///< "true" if it's not allowed to lose life
		bool				mCanDoContaminationQuickTest;	///< "true" in the beginning, "false" after quicktest is doing once

		// Immunity and resistance
		std::vector<uint32> mIllnessImmunities;			///< Immunities against injuries, vector holds IDs of injuries that failed to infect this entity in the past
		ImmunityBitmask		mImmunities;				///< Set of damage immunities that this entity has
		float				mResistances[damagetype::NUM_DAMAGETYPES];	///< A array of resistances the entity has

		// Proxies
		qsf::JobProxy		mSimulationJobProxy;		///< Regular simulation job proxy; for updates once a game tick
		qsf::JobProxy		mSpreadInfectionJobProxy;	///< Special simulation job proxy; for spreading injuries to other healthcomponents
		qsf::DebugDrawProxy mDebugDrawProxy;			///< Only for debug

		qsf::Time	mElapsedTimeAfterLastHeatEnergyReceived;	///< Holds the elapsed time after the last received heat energy occurrence

		// Intermediate states
		bool		mCanSendReceivedHeatEnergyMessage;	///< Indicates if we can send the message "received heat energy" (gets reseted after some time passed without receiving heat energy)

		// Hint
		qsf::Time	mAttackHintWaitingTime;				///< Waiting time until the next attack int is shown

		// Status icons
		StatusOverlayComponent::StatusIconType	mHealthIconType;	///< Shows if person is sick or contaminated
		qsf::Time								mTimeShowStatusIconAttack;
		qsf::Time								mTimeShowStatusIconWeapon;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/health/HealthComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HealthComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::HealthComponent::ImmunityType)
