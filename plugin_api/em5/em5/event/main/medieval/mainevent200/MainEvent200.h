// Copyright (C) 2012-2018 Promotion Software GmbH


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
#include <qsf/math/Transform.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class ParameterGroup;
}
namespace em5
{
	class MedievalPreludeScreen;
	class BlockAreaWithUnits;
	class OrderInfo;
	class Player;
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
	*    EMERGENCY 2017 main event 200 "Medieval Hamburg"
	*
	*  @note
	*    The word "pyre" is the burning wood pile with witches, so stake would fit better
	*/
	class MainEvent200 : public MainEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent200" unique freeplay event ID

		struct ArsonistTargetAssigment
		{
			ArsonistTargetAssigment(std::string arsonistEventTag, std::string unparsedString);

			std::string	mArsonistEventTag;
			int	mArrestedPersonTriggerValue;
			std::vector<std::string>	mTargets;
			bool	mActive;
		};

		struct UnitPosition
		{
			qsf::WeakPtr<qsf::Entity> mUnit;
			qsf::Transform mStartTransform;
			qsf::Time mStartDelay;	// Seconds before the unit starts move to start position
			bool mIsStarted;
			bool mCanStart;
		};

		struct StoneThrower
		{
			qsf::WeakPtr<qsf::Entity>	mEntity;
			qsf::Time	mWaitTime;
			bool		mActive;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent200();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent200();


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
		virtual void onStartIntroRequest() override;
		virtual void onIntroStart(const qsf::MessageParameters& parameters) override;
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;
		virtual void onPrologueFinished(const qsf::MessageParameters& parameters) override;
		virtual void onUpdateMusic(const qsf::Time& timePassed) override;
		virtual uint32 getEventNumber() override;
		virtual std::string getCamStartEventTag() override;
		virtual void showBriefingSupervisorMessage() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setMedievalGraphics(bool medievalMode);

		void blockInput(bool block);
		void onMedievalPreludeScreenPlaybackFinished();

		void gatherEventEntities();

		void registerGuardsmanForPlayer();

		void setupGuardsman(qsf::Entity& entity, OrderInfo& guardsmanOrderInfo, Player& localPlayer);
		void setupGangster(qsf::Entity& gangsterEntity, std::vector<std::string>& targets);

		void startFadeOutForFanaticStart();
		bool updateGuardsmanSpawningFromBarracks(UnitPosition& unitPosition, qsf::Time timeElapsed);
		void updateStoneThrower(const qsf::Time& timePassed);
		void startSingleFanatic();
		void startFanatics();
		void startFleeing();
		void startGuardsmanSupport();
		void winall();	// Debug

		void onUpdateInfoScreen(const qsf::JobArguments& jobArguments);
		void fadeoutComplete(const qsf::MessageParameters& parameters);
		void onFadeoutAfterGuardsmanPlacement(const qsf::MessageParameters& parameters);
		void onFadeInAfterGuardsmanPlacement(const qsf::MessageParameters& parameters);

		void onWitchRescued(const qsf::MessageParameters& parameters);
		void onArrestGangster(const qsf::MessageParameters& parameters);
		void onBringGangsterHome(const qsf::MessageParameters& parameters);

		void makeAllPersonsImmune();

		// Gangster/Arsonist assignment
		void buildArsonistTargetAssigmentFromData();
		void updateArsonistStarting();

		// Audio
		/**
		*  @brief
		*    Is sets the AmbientPolygon behind mCrowdAudio to defined states.
		*  @param[in] state
		*    0 = slience, no audio is playing, 1 = crowd is cheering, 2 = crowd is angry
		*/
		void setAmbientAudio(int state);

		void activateStakeAndWellObjects(bool active);

		/**
		*  @brief
		*    Called as soon as there's a property change inside the settings group manager
		*
		*  @param[in] parameterGroup
		*    Parameter group instance the changed property is in
		*  @param[in] propertyId
		*    Unique class property ID the component property which values was changed
		*/
		void onSettingsGroupPropertyChanged(const qsf::ParameterGroup& parameterGroup, uint32 propertyId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Setup data

		// Internal data
		MedievalPreludeScreen* mMedievalPreludeScreen;	///< Valid while the event is running, can be a null pointer, destroy the instance if you no longer need it
		qsf::Time	mTimeWaitAfterGuardsmanSpawn;
		qsf::Time	mTimeWaitFanaticActive;
		qsf::Time	mTimeWaitBlackout;
		qsf::Time	mIgniteTimeStake;
		qsf::Time	mIgniteTimeObject;
		int	mNumberArrestGangster;
		bool	mPersonsCanFlee;
		bool	mHintBurningCanTrigger;
		bool	mGuardsmanSupportCanArrive;
		qsf::Time	mTimeGuardsmanSupportArrived;

		std::vector<qsf::WeakPtr<qsf::Entity>>	mPyreEntities;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mFanatics;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mArsonistEntities;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mFleeEntities;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mDespawnPointEntities;
		std::vector<StoneThrower>				mStoneThrowers;
		qsf::WeakPtr<qsf::Entity>				mRallyPoint;
		qsf::WeakPtr<qsf::Entity>				mBarracksPoint;
		qsf::WeakPtr<qsf::Entity>				mCrowdAudio;
		qsf::WeakPtr<qsf::Entity>				mCrowdAngryAudio;
		qsf::WeakPtr<qsf::Entity>				mSingleFanatic;

		BlockAreaWithUnits& mBlockAreaWithUnits;

		enum State
		{
			State_Intro,
			State_SelectGuardsman,
			State_OpenInfoScreen,
			State_CloseInfoScreen,
			State_WaitTillGuardsmanSpawn,
			State_BringGuardsmanInPosition,
			State_FadeInForFanaticStart,
			State_FadeOutForFanaticStart,
			State_ArrestSingleFanatic,
			State_ArrestFanatics,
			State_Wait_for_fadeout
		};
		State			mCurrentState;

		std::vector<UnitPosition>	mUnitPositions;

		qsf::JobProxy	mRegisterOpenHelpWindowJobProxy;	// Only active in specific gamestate
		qsf::MessageProxy	mRescueWitchFromPyreMessageProxy;
		qsf::MessageProxy	mArrestGangsterMessageProxy;
		qsf::MessageProxy	mBringGangsterHomeMessageProxy;
		qsf::MessageProxy	mFadingMessageProxy;

		std::vector<ArsonistTargetAssigment>	mArsonistTargetAssigment;

		// Arsonist targets Fanatic
		std::string		mArsonistTargetsFanatic1;
		std::string		mArsonistTargetsFanatic2;
		std::string		mArsonistTargetsFanatic3;
		std::string		mArsonistTargetsFanatic4;

		// Arsonist targets Arsonist
		std::string		mArsonistTargetsArsonist1;
		std::string		mArsonistTargetsArsonist2;
		std::string		mArsonistTargetsArsonist3;
		std::string		mArsonistTargetsArsonist4;
		std::string		mArsonistTargetsArsonist5;
		std::string		mArsonistTargetsArsonist6;
		std::string		mArsonistTargetsArsonist7;
		std::string		mArsonistTargetsArsonist8;
		std::string		mArsonistTargetsArsonist9;
		std::string		mArsonistTargetsArsonist10;
		std::string		mArsonistTargetsArsonist11;
		std::string		mArsonistTargetsArsonist12;
		std::string		mArsonistTargetsArsonist13;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent200)
