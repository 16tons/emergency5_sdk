// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/main/em20years/Em20MainEventBase.h"


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
	*    EMERGENCY 20 years edition mission 02 "Illegal Street Race"
	*/
	class MainEvent302 : public Em20MainEventBase
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent302" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent302();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent302();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
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
		//[ Timers                                                ]
		//[-------------------------------------------------------]
		virtual void onTimerTriggered(uint32 timerId) override;

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
		virtual void onIntroFinished(const qsf::MessageParameters& parameters) override;

		//[-------------------------------------------------------]
		//[ Regular update                                        ]
		//[-------------------------------------------------------]
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;

		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum class CameraFlight
		{
			NONE,
			STREET_RACE_START
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene();
		void startObjectives();

		void onCameraFlightStopped(const qsf::MessageParameters& parameters);
		void onCutsceneEvent(const qsf::MessageParameters& parameters);
		void onCutsceneEnd(const qsf::MessageParameters& parameters);
		void onCutsceneFinish(const qsf::MessageParameters& parameters);
		void onGangsterWaypoint(const qsf::MessageParameters& parameters);
		void onGangsterEscaped(const qsf::MessageParameters& parameters);

		void startCameraFlight(CameraFlight cameraFlight, qsf::Entity& targetEntity);
		void startRace();

		void makeCarTowable(qsf::Entity& entity);
		void enablePlayerUnitMovement(bool enable);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time mTimeUntilRaceStarts;

		// Internal
		qsf::WeakPtr<qsf::Entity> mTriggerBox;
		qsf::WeakPtr<qsf::Entity> mMinimapArea;
		qsf::WeakPtr<qsf::Entity> mCameraPositionForCutscene;
		qsf::WeakPtr<qsf::Entity> mRacer01Car;
		qsf::WeakPtr<qsf::Entity> mRacer02Car;
		qsf::WeakPtr<qsf::Entity> mRacer02CarTarget;
		qsf::WeakPtr<qsf::Entity> mRacer02;

		qsf::MessageProxy mCameraFlightStoppedMessageProxy;
		qsf::MessageProxy mCutsceneEventMessageProxy;
		qsf::MessageProxy mCutsceneEndMessageProxy;
		qsf::MessageProxy mCutsceneFinishMessageProxy;
		qsf::MessageProxy mGangsterWaypointMessageProxy;
		qsf::MessageProxy mGangsterEscapedMessageProxy;

		CameraFlight mActiveCameraFlight;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent302)
