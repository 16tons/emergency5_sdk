// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BuildingComponent;
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
	*    EMERGENCY 5 freeplay event class for "Arsonist"
	*/
	class EM5_API_EXPORT ArsonistEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::ArsonistEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ArsonistEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ArsonistEvent();

		/**
		*  @brief
		*    Get the ID of the gangster
		*/
		uint64 getGangsterId() const;

		/**
		*  @brief
		*    Set the arsonist
		*/
		void setArsonist(const qsf::Entity& entity);

		/**
		*  @brief
		*    Define whether only empty buildings shall be set on fire
		*/
		void setFirstBuildingMustHaveBalcony(bool firstBuildingMustHaveBalcony);

		void setCheeringTime(const qsf::Time& cheeringTime);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual void hintCallback(Observer& hintObserver);
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		BuildingComponent* findBuildingCandidate(qsf::Entity& arsonist) const;
		bool checkBuildingCandidate(qsf::Entity& building) const;
		void startObjectives(const qsf::Entity& targetEntity);
		void startHintObservers();
		bool checkIfIsHealthy(const qsf::Entity& victim) const;
		void escape(qsf::Entity& gangster) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64 mGangsterId;
		uint64 mCurrentTargetId;
		uint64 mBuildingsBurnt;
		bool mFirstBuildingMustHaveBalcony;	///< This is a special case for Arsonist02
		bool mPerpetrated;
		bool mRescue;
		qsf::Time mCheeringTime;
		bool mDontShootTriggered;

		enum NextAction
		{
			UNDEFINED = 0,
			GO_TO_NEXT_BUILDING,	///< Go to closest building
			SET_BUILDING_ON_FIRE,	///< Set that building on fire
			WATCH_AND_CHEER,		///< Step away from the building and cheer
			DO_NOTHING
		} mNextAction;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ArsonistEvent)
