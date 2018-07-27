// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/base/WeakPtr.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/time/Time.h>


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
	*    EMERGENCY 5 freeplay event "building nearly collapse"
	*    This is also a tutorial event	// TODO(co) "This is also a tutorial event" What does this mean?
	*/
	class EM5_API_EXPORT BuildingNearlyCollapseEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::BuildingNearlyCollapseEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BuildingNearlyCollapseEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BuildingNearlyCollapseEvent();

		/**
		*  @brief
		*    Initialization used by factory
		*
		*  @param[in] targetBuilding
		*    Entity for the building which should collapse
		*  @param[in] isTutorial
		*    "true" if special tutorial hints are playing
		*/
		void init(qsf::Entity& targetBuilding, bool isTutorial);

		void setAmbientLayerName(const std::string& layerName);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FailEvacuateBuildingTimer, qsf::Time, mFailEvacuateBuildingTimer);
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FailRepairBuildingTimer, qsf::Time, mFailRepairBuildingTimer);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual bool checkEventConfiguration() override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onPersonRescueFromBuilding(const qsf::MessageParameters& parameters);
		void onBreakBuildingDoor(const qsf::MessageParameters& parameters);
		void onBuildingRepaired(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::Entity> mTargetBuilding;	///< Building to collapse
		bool mIsTutorial;	// "true" if special tutorial hints are displayed

		qsf::MessageProxy	mRescuedFromBuildingMessageProxy;
		qsf::MessageProxy	mRepairBuildingMessageProxy;

		std::string mAmbientLayerName;
		bool		mIsBuildingEvacuated;
		bool		mIsBuildingReparied;

		// Event fail
		qsf::Time mFailEvacuateBuildingTimer;
		qsf::Time mFailRepairBuildingTimer;

		// Tutorial
		qsf::Time			mTutorialHint1Timer;
		bool				mTutorialHint1Played;
		bool				mTutorialHint2Played;
		qsf::MessageProxy	mBreakBuildingDoorMessageProxy;
		bool				mTutorialHint3Played;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuildingNearlyCollapseEvent)
