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
	*    EMERGENCY 20 years edition mission 10 "Nuclear Power Plant"
	*/
	class MainEvent310 : public Em20MainEventBase
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class MainEventMetaclasses;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MainEvent310" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MainEvent310();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MainEvent310();


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
	//[ Protected virtual em5::Em20MainEventBase methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUnitSetup(qsf::Entity& entity, uint32 unitTypeId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene();
		void startObjectives();

		void updateContamination(const qsf::Time& timePassed);

		void onCheckGuidePersonTarget(const qsf::MessageParameters& parameters);
		void onPersonExitsAfterDecontamination(const qsf::MessageParameters& parameters);
		void onPersonExitedAfterDecontamination(const qsf::MessageParameters& parameters);
		void onPersonInjured(const qsf::MessageParameters& parameters);
		void onObjectStartsBurning(const qsf::MessageParameters& parameters);
		void onBusReadyForUnload(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		qsf::Time mTimeForSlowContamination;
		qsf::Time mTimeForFastContamination;

		// Internal
		qsf::WeakPtr<qsf::Entity> mAreaFastContamination;
		qsf::WeakPtr<qsf::Entity> mAreaSafeZone;
		qsf::WeakPtr<qsf::Entity> mTent;
		qsf::WeakPtr<qsf::Entity> mTentUnload;

		qsf::Time mTimeUntilNextContaminationUpdate;
		boost::container::flat_map<uint64, float> mContaminationProgress;	// Contamination progress in [0.0f, 1.0f] by entity ID

		qsf::MessageProxy mCheckGuidePersonTargetMessageProxy;
		qsf::MessageProxy mPersonExitsAfterDecontaminationMessageProxy;
		qsf::MessageProxy mPersonExitedAfterDecontaminationMessageProxy;
		qsf::MessageProxy mPersonInjuredMessageProxy;
		qsf::MessageProxy mObjectStartBurningMessageProxy;
		qsf::MessageProxy mBusReadyForUnloadMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MainEvent310)
