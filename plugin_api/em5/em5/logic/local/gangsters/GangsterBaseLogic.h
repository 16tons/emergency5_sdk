// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/equipment/weapon/WeaponConfiguration.h"

#include <qsf/component/base/TransformComponent.h>
#include <qsf/job/JobProxy.h>
#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/map/Entity.h>
#include <qsf/math/EnumBitmask.h>

#include <deque>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
}
namespace em5
{
	class GangsterType;
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
	*    Base game logic for gangster
	*/
	class EM5_API_EXPORT GangsterBaseLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::GangsterBaseLogic" game logic type ID

		enum State
		{
			STATE_NORMAL,		// The gangster is currently capable of moving, fighting, fleeing, idling...
			STATE_ARRESTED		// The gangster was arrested and cannot do anything

			// Note: For querying special behavior, check isCurrentlyFighting() or isCurrentlyFleeing()!
		};

		enum EscapeType
		{
			ESCAPE_FOOT_LONG,
			ESCAPE_FOOT_SHORT,
			ESCAPE_HIDE_FOOT,
			ESCAPE_CAR_LONG,
			ESCAPE_CAR_SHORT,
		};

		enum class AutoReaction
		{
			NONE,
			ATTACK,
			ESCAPE_SHORT,
			ESCAPE_LONG
		};

		typedef qsf::EnumBitmask<uint16, weapon::Weapon> WeaponFlagSet;

		struct PossibleTarget
		{
			qsf::Entity* mEntity;
			qsf::StringHash mEventTag;
			uint32 mPriority;
			float mDistance;

			// Need an default constructor otherwise vector::emplace_back(<val>, <val2>, <val3>) doesn't work...
			PossibleTarget();

			// Need an constructor otherwise vector::emplace_back(<val>, <val2>, <val3>) doesn't work...
			PossibleTarget(qsf::Entity& entity, const qsf::StringHash& eventTag, uint32 priority, float distance);
		};
		// Legacy definition
		typedef PossibleTarget SpottedUnit;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GangsterBaseLogic();

		/**
		*  @brief
		*    Constructor
		*/
		explicit GangsterBaseLogic(uint32 gameLogicId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GangsterBaseLogic();

		void applyGangsterType(const GangsterType& gangsterType);

		/**
		*  @brief
		*    Return the GangsterLogic class and, if existing, the gangster type name
		*/
		std::string getDebugGangsterName() const;

		//[-------------------------------------------------------]
		//[ Property getters and setters                          ]
		//[-------------------------------------------------------]
		inline WeaponFlagSet getEquippedWeaponTypes() const;
		inline void setEquippedWeaponTypes(WeaponFlagSet weapons);

		inline State getCurrentState() const;
		void setCurrentState(State newState);

		bool getAsGangsterFlagged() const;
		void setAsGangsterFlagged(bool isGangster);

		inline AutoReaction getIdleReaction() const;
		inline void setIdleReaction(AutoReaction autoReaction);

		inline AutoReaction getAutoTargetReaction() const;
		inline void setAutoTargetReaction(AutoReaction autoReaction);

		inline bool getAutoAggression() const;
		inline void setAutoAggression(bool autoAggression);

		inline float getAutoAggroRange() const;
		inline void setAutoAggroRange(float autoAggroRange);

		inline float getAutoDeAggroRange() const;
		inline void setAutoDeAggroRange(float autoAggroRange);

		inline bool getResistArrest() const;
		inline void setResistArrest(bool resistArrest);

		weapon::WeaponConfiguration* getConfigurationForWeapon(weapon::Weapon usedWeapon);
		const weapon::WeaponConfiguration* getConfigurationForWeapon(weapon::Weapon usedWeapon) const;

		/**
		*  @brief
		*    Sets the attack distance required for a specific weapon.
		*
		*  @remarks
		*    Currently only RIFLE and PISTOL are supported
		*/
		inline void setWeaponAttackDistance(weapon::Weapon weapon, float distance);
		inline float getWeaponAttackDistance(weapon::Weapon weapon) const;

		void addAutoTargetEventTag(const qsf::StringHash& eventTag, uint32 priority);

		inline qsf::Time getCelebrateWinTime() const;
		inline void setCelebrateWinTime(qsf::Time celebrateWinTime);

		/**
		*  @brief
		*    Sets the chain reaction feature active. This will listen to injured gangster in aggro range and start autoreact on this targert.
		*
		*  @remakrs
		*    Acivates a message proxy and listen to message EM5_COMPONENT_PERSON_GETS_SHOOT
		*/
		void setChainReaction(bool active);
		inline bool getChainReaction();


		//[-------------------------------------------------------]
		//[ Behavior                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    The gangster attacks the given target with the given weapon
		*
		*  @param[in] targetEntity
		*    Entity which should be attacked by the gangster
		*  @param[in] usedWeapon
		*    The weapon which should be used for the attack
		*/
		void attack(qsf::Entity& targetEntity);

		// Gangster escape method
		void escape(EscapeType escapeType);

		inline void setEscapeTargetTag(const std::string& escapeTargetTag);

		/**
		*  @brief
		*	 Stores the gangster logic that belonged to this gangster before the current one
		*
		*  @note
		*    - It is set automatically when change[Gangster]Logic() is called
		*/
		void setLastGangsterLogic(uint32 lastLogicId);

		qsf::Entity* getCurrentAutoAggresionTarget();


		//[-------------------------------------------------------]
		//[ Weapon display                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Call to let the currently active gangster weapon vanish (if any)
		*/
		void hideCurrentWeapon();

		/**
		*  @brief
		*    Call to create a new weapon from prefab and make it visible (equipped)
		*
		*  @note
		*    - This calls hideCurrentWeapon()
		*/
		void showWeapon(const std::string& equipmentPrefab);

		//[-------------------------------------------------------]
		//[ Waypoints and patrolling                              ]
		//[-------------------------------------------------------]
		inline bool getPatrolOnWaypoints() const;
		inline void setPatrolOnWaypoints(bool patrol);

		/**
		*  @brief
		*    Clear list of waypoints
		*/
		void clearWaypoints();

		/**
		*  @brief
		*    Add a waypoint to the end of the waypoint list
		*/
		void addWaypoint(qsf::Entity& waypointEntity, uint32 targetPointProviderId = qsf::getUninitialized<uint32>(), uint32 movementModeId = qsf::getUninitialized<uint32>());
		void addWaypoint(glm::vec3 waypointPosition, uint32 movementModeId = qsf::getUninitialized<uint32>());


	//[-------------------------------------------------------]
	//[ Public virtual methods                                ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Returns the moving speed with which the gangster should move
		*/
		inline virtual float getMovingSpeed() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    If overriding this function in derived class, make sure to call this function to make sure startup process is correct
		*/
		virtual bool onStartup() override;

		/**
		*  @brief
		*    If overriding this function in derived class, make sure to call this function to make sure shutdown process is correct
		*/
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Helper method to replace the current gangster game logic (When the gangster should change his behavior)
		*/
		bool changeLogic(const qsf::StringHash& newLogicId);

		/**
		*  @brief
		*    Helper method to check whether the gangster's current action is a fleeing action
		*/
		bool isCurrentlyFleeing() const;

		/**
		*  @brief
		*    Helper method to check whether the gangster's current action is a fighting action, i.e. Shoot, ThrownWeapon, CloseQuartersCombat
		*/
		bool isCurrentlyFighting() const;

		/**
		*  @brief
		*    Give all civil persons around a little fear
		*/
		void fearNearbyCivilists();

		/**
		*  @brief
		*    If escaping, check if nearly police is attacking the gangster and tell the escape action to slowdown movement. So player units a chance to catch them.
		*/
		void updateCatchedByPolice();


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		struct Waypoint
		{
			qsf::WeakPtr<qsf::Entity> mEntity;
			uint32 mTargetPointProviderId;
			uint32 mMovementModeId;
			uint32 mTriesToReachLeft;
			glm::vec3 mPosition;
		};

		typedef boost::container::flat_map<qsf::StringHash, uint32> PriorityByEventTagMap;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GangsterBaseLogic methods      ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called once a tick
		*
		*  @remarks
		*    The method is not called if gangster is inside a container or cannot act right now (e.g. when injured or arrested)
		*/
		virtual void onSimulationUpdate(const qsf::JobArguments& jobArguments) {}

		/**
		*  @brief
		*    Called in case gangster has no action right now
		*
		*  @remarks
		*    The method is called every simulation step in case there is no action left in the action plan.
		*    It is called after the simulation update.
		*    Implementation in sub-classes is optional.
		*/
		virtual void onIdle() {}

		/**
		*  @brief
		*    Called when there were any units spotted which are considered targets of the gangster
		*
		*  @remarks
		*    The method is called every simulation step in case there are units in the target range. Else onNoUnitsSpotted() will be called instead.
		*    Injured, Dead, Burning and Destroyed units are _excluded_!
		*
		*  @note
		*    - This only gets called at all if "mUseOldSpottedUnitsLogic" is set to "true"
		*/
		virtual void onUnitsSpotted(std::vector<SpottedUnit>& sightedPoliceUnits) {}

		/**
		*  @brief
		*    This is the counterpart to onUnitsSpotted. Implement it (not in here, override it!) if you need to be notified when no police units are in range
		*
		*  @remarks
		*    The method is called every simulation step in case there are NO units in the target range. Else onUnitsSpotted() will be called instead with the found units
		*
		*  @note
		*    - This only gets called at all if "mUseOldSpottedUnitsLogic" is set to "true"
		*/
		virtual void onNoUnitsSpotted() {}

		/**
		*  @brief
		*    Called when the next waypoint was just reached
		*/
		virtual void onWaypointReached(const Waypoint& waypoint) {}

		virtual void onPersonGetsShoot(const qsf::MessageParameters& parameters);

		virtual void onGunShot(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job update
		void updateSimulation(const qsf::JobArguments& jobArguments);
		void updateMoveOnWaypoints();
		void updateLegacyAutoAggression();
		void updateAutoAggression();

		void findPossibleTargets(std::vector<PossibleTarget>& outTargets, float radius, const PriorityByEventTagMap& targetPrioritiesByEventTag);
		bool checkTargetEntity(qsf::Entity& entity);
		void removeGangsterType();

		void setCurrentAutoTarget(const PossibleTarget& possibleTarget);
		void loseCurrentAutoTarget();

		void autoAggressionReactOnTargets(std::vector<PossibleTarget>& possibleTargets);
		void filterEquippedWeapons();
		bool attackTarget(qsf::Entity& targetEntity);
		bool attackWithWeapon(qsf::Entity& targetEntity, weapon::Weapon usedWeapon);

		void startupInternal();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		const GangsterType*						mGangsterType;				///< Gangster type; may be a null pointer, do not destroy the instance

		WeaponFlagSet							mEquippedWeaponTypes;		///< Indicates which weapons the gangster has
		uint32									mLastGangsterLogicId;		///< In case this gangster logic was changed, this variable holds the former logic
		qsf::WeakPtr<qsf::TransformComponent>	mTransformComponent;		///< Caching of gangster's transform component
		qsf::WeakPtr<qsf::ActionComponent>		mActionComponent;			///< Caching of gangster's action component
		bool									mSetGangsterFlagOnStartup;	///< If set, the gangster will be visible as gangster to the player from the beginning
		bool									mAlwaysShowWeapon;			///< Flag whether this gangster type show his weapon/ carries it visibly
		bool									mUseOldSpottedUnitsLogic;	///< Backwards compatibility mode
		bool									mConsiderAllUnitsAsTarget;	///< Flag whether this gangster type should not only attack police units, but any unit
		bool									mChainReaction;				///< If set, gangster will detect if nearby gangster are attacked
		std::string								mEscapeTargetTag;			///< Event tag of the target spawn point to escape to

		AutoReaction							mIdleReaction;				///< What to do when idling
		AutoReaction							mAutoTargetReaction;		///< Reaction to targets in aggro range
		bool									mAutoAggression;			///< Lets a gangster search for targets to attack
		float									mAutoAggroRange;			///< Range this gangster searches to targets
		float									mAutoDeAggroRange;			///< Range where this gangster ignored the target

		qsf::WeakPtr<qsf::Entity>				mCurrentAutoTarget;					///< The current target
		uint32									mCurrentAutoTargetPriority;
		PriorityByEventTagMap					mAutoTargetPrioritiesByEventTag;	///< Event tags we want to attack, and their priorities (usually between 1 and 5; the higher, the more important)

		qsf::Time								mCelebrateWinTime;				///< Time to celebrate a win
		weapon::WeaponConfiguration				mWeaponConfigurationForPistol;	///< Contains all weapon-specific settings for pistol usage
		weapon::WeaponConfiguration				mWeaponConfigurationForRifle;	///< Contains all weapon-specific settings for rifle usage

		std::deque<Waypoint>					mWaypoints;						///< Waypoints the gangster will try to go to if he has nothing better to do
		bool									mPatrolOnWaypoints;				///< If set, the gangster will repeatedly cycle through his waypoints, and won't run in between

		bool									mNeedsUpdateInContainer;		///< If "false", the gangster won't get updates if inside a container


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State			mCurrentState;			///< Current state in which the "gangster" is
		qsf::JobProxy	mSimulationJobProxy;	///< Regular simulation job proxy; for updates once a game tick
		bool			mResistArrest;			///< If "false", the gangster will just giving without a fight when being arrest

		qsf::MessageProxy	mPersonGetsShootMessageProxy;
		qsf::MessageProxy	mGunshotMessageProxy;


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
#include "em5/logic/local/gangsters/GangsterBaseLogic-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterBaseLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::GangsterBaseLogic::AutoReaction)
