// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"

#include <qsf/job/JobProxy.h>
#include <qsf/base/WeakPtr.h>


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
	*    EMERGENCY 2017 main event 203 "Ausheben einer Terrorzelle"
	*/
	class MainEvent203 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;	// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;	///< "em5::MainEvent203" unique freeplay event ID

		// For matching gangster and waypoints
		struct GangsterWaypointData
		{
			bool loop;
			qsf::WeakPtr<qsf::Entity> person;
			qsf::WeakPtr<qsf::Entity> waypoint;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent203();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent203();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Life cycle                                            ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual bool onFailure(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Points evaluation                                     ]
		//[-------------------------------------------------------]
		virtual uint32 calculatePointGain(EventResult& eventResult) override;

		//[-------------------------------------------------------]
		//[ Callbacks from observers                              ]
		//[-------------------------------------------------------]
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual void hintCallbackWithMessage(Observer& hintObserver, const qsf::MessageParameters& parameters) override;

		//[-------------------------------------------------------]
		//[ Miscellaneous                                         ]
		//[-------------------------------------------------------]
		virtual const qsf::Entity* getFocusEntity() override;

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainEvent methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onIntroStart(const qsf::MessageParameters& parameters) override;
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;

		//[-------------------------------------------------------]
		//[ Regular update                                        ]
		//[-------------------------------------------------------]
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;

		virtual uint32 getEventNumber() override;
		virtual std::string getCamStartEventTag() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void gatherEventEntities();
		void startObjectives();
		void startHintObservers();

		void startHouseExplosion();
		void onHouseExplosionDelay();
		void showSparkleEffect(bool active);
		void startFoundGangsterInBushes();
		void showHiddenTerrorists(bool active);

		void onBuriedFound(const qsf::MessageParameters& parameters);
		void onGunShot(const qsf::MessageParameters& parameters);
		void onEboxRepaired(const qsf::MessageParameters& parameters);
		void onTookPersonOutside(const qsf::MessageParameters& parameters);

		void patrolOnPath(qsf::Entity& personEntity, qsf::Entity& pathEntity, bool runToTarget);
		void startPatroling();
		void startPatroling(qsf::Entity& gangster, bool runToTarget);

		bool checkCollisionForWarningAreaById(qsf::Entity& unitentity, int index);
		void activateWarningArea(int warningAreaIndex, bool active);
		void activateGangsterOnRoof(int houseIndex);
		qsf::Entity* getWarningAreaById(int warningAreaIndex);
		void setupGangster(qsf::Entity& gangster);
		void setupTerroristHouse(qsf::Entity& terroristHouse);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Setup
		float	mPersonInjuredRange;

		// Settings, connected to camp
		qsf::Time	mHint01WaitTime;
		qsf::Time	mHint02WaitTime;
		int			mMaxDeadCivilians;
		int			mMaxDeadUnitPerson;
		int			mMaxBurningObjects;

		// Internal data
		qsf::WeakPtr<qsf::Entity>	mTerroristHouse1;
		qsf::WeakPtr<qsf::Entity>	mTerroristHouse2;
		qsf::WeakPtr<qsf::Entity>	mTerroristHouse3;
		qsf::WeakPtr<qsf::Entity>	mTerroristHouseStartExplosion;
		std::vector<qsf::WeakPtr<qsf::Entity>> mTerroristHouses;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mExplosionStartBurnings;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mExplosionDropPositions;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mExplosionDebris;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mExplosionDebrisPerson;
		qsf::WeakPtr<qsf::Entity>	mToxicCloud;
		qsf::WeakPtr<qsf::Entity>	mEboxEntity;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mEboxSparkle;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mTerroristsHidden;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mTerrorists;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mTerroristsInhouse;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mTerroristsOnHouse;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mTerroristsDeco;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mWarningArea;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mTerroristsWithKneeingAnimation;		// Redundant to other lists, just give them animation at start
		std::vector<qsf::WeakPtr<qsf::Entity>>	mTerroristsWithManipulatingAnimation;	// Redundant to other lists, just give them animation at start
		qsf::WeakPtr<qsf::Entity>	mDummyContainerEntity;	// Generated Entity, used to "hide" policeman via explosion
		qsf::WeakPtr<qsf::Entity>	mTweetCameraEntity;
		qsf::WeakPtr<qsf::Entity>	mTweetPersonEntity;

		// For gangster movement
		// Some of the gangsters above has event tags for movement. The tags have identifier for patrol ways
		// The patrol waypoints also have numbers for a 1:1 matching of gangster to waypoint
		std::vector<qsf::WeakPtr<qsf::Entity>>	mPatrolLoop;	// Gangster Entities with the order to loop the movement for his waypoint
		std::vector<qsf::WeakPtr<qsf::Entity>>	mPatrolSingle;	// Gangster Entities with the order to walk the waypoints and stay there
		std::vector<qsf::WeakPtr<qsf::Entity>>	mPatrolWayPoint;	// Waypoints
		qsf::WeakPtr<qsf::Entity> mPatrolHiddenWaypointEntity;	// Waypoints for hidden gangster

		std::vector<GangsterWaypointData> mGangsterWaypointData;

		bool mHouseExplosionStarted;
		bool mHouseExplosionDelayFinished;
		qsf::Time mHouseExplosionDelayTimer;
		bool mEboxRepaired;
		bool mFoundGangsterInBushes;
		qsf::Time mHiddenGangsterAttackTime;
		bool mHiddenGangsterAttacked;

		bool	mWarningAreaHidden1;
		bool	mWarningAreaHidden2;
		bool	mWarningAreaHidden3;
		std::vector<uint64>	mAttackedBuildings;

		qsf::MessageProxy mBuriedFoundMessageProxy;
		qsf::MessageProxy mEboxRepairedProxy;
		qsf::MessageProxy mAttackedBuildingsProxy;

		bool	mCanPlayHint01;
		bool	mCanPlayHint02;

		// Music level 1 is active if the gangster recognize the player units. Triggers only once
		bool	mCanTriggerMusicLevel1;
		qsf::MessageProxy mGunShotMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent203)
