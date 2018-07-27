// Copyright (C) 2012-2017 Promotion Software GmbH


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
	*    EMERGENCY 20 years edition mission 05 "Supertanker"
	*/
	class MainEvent305 : public Em20MainEventBase
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent305" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent305();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent305();


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
		//[-------------------------------------------------------]
		//[ Regular update                                        ]
		//[-------------------------------------------------------]
		virtual void onUpdateMainEvent(const qsf::Time& timePassed) override;

		virtual uint32 getEventNumber() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene();
		void startObjectives();

		void onUnitSpawn(const qsf::MessageParameters& parameters);
		void onCheckBoatTarget(const qsf::MessageParameters& parameters);
		void onObjectStartsBurning(const qsf::MessageParameters& parameters);
		void onBuriedFound(const qsf::MessageParameters& parameters);
		void onPersonPlacedInHeli(const qsf::MessageParameters& parameters);
		void onCutsceneFinished(const qsf::MessageParameters& parameters);

		void makeDrowningPerson(qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		float mFireboatMoveSpeed;
		float mFireboatAcceleration;
		float mFireboatRange;
		qsf::Time mDelayForBurnDelayed;

		// Internal
		float mOriginalFireboatRange;

		struct Sailor
		{
			qsf::WeakPtr<qsf::Entity> mEntity;
		};
		struct ShipPart
		{
			uint32 mNumber = 0;
			qsf::WeakPtr<qsf::Entity> mEntity;
			std::vector<Sailor> mSailors;
			bool mBurning = false;
		};
		std::vector<ShipPart> mShipParts;

		qsf::MessageProxy mUnitSpawnMessageProxy;
		qsf::MessageProxy mCheckBoatTargetMessageProxy;
		qsf::MessageProxy mObjectStartsBurningMessageProxy;
		qsf::MessageProxy mBuriedFoundMessageProxy;
		qsf::MessageProxy mPersonPlacedInHeliMessageProxy;
		qsf::MessageProxy mCutsceneFinishedMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent305)
