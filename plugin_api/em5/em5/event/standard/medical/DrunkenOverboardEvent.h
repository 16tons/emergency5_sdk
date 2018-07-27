// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/message/MessageProxy.h>


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
	*    EMERGENCY 5 freeplay event "drunken overboard"
	*/
	class EM5_API_EXPORT DrunkenOverboardEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::DrunkenOverboardEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DrunkenOverboardEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DrunkenOverboardEvent();

		/**
		*  @brief
		*    Get the ID of the target person
		*/
		uint64 getTargetPersonId() const;

		/**
		*  @brief
		*    Set the target entity id
		*/
		void setTargetPersonId(uint64 targetPersonId);

		/**
		*  @brief
		*    Get the ID of the target ship
		*/
		uint64 getTargetShipId() const;

		/**
		*  @brief
		*    Set the target ship id
		*/
		void setTargetShipId(uint64 targetShipId);


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
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives(const qsf::Entity& targetEntity);
		void startHintObservers();
		void onDivingSpotCreated(const qsf::MessageParameters& parameters);
		void findEntityInDivingSpot(const qsf::MessageParameters& parameters);
		void onTimeIsUp(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64				mTargetPersonEntityId;		///< ID of the drunken person which falls overboard
		uint64				mTargetShipEntityId;
		uint64				mDivingSpotEntityId;
		qsf::Time			mMaxRunningTime;			///< Max time to rescue the person (this is not part of the gamedesign)
		qsf::MessageProxy	mMessageProxy;
		qsf::MessageProxy	mTimerMessageProxy;

		// Internal
		bool				mIsPersonUnderwater;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DrunkenOverboardEvent)
