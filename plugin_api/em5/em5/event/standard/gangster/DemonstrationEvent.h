// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/message/MessageSystem.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Layer;
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
	*    EMERGENCY 5 freeplay event class for the demonstration event
	*/
	class EM5_API_EXPORT DemonstrationEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::DemonstrationEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Static checks to evaluate if we can create the event in the first place
		*/
		static bool canSpawnEntitiesCollisionFree(const qsf::Layer& layer, qsf::Map& map);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DemonstrationEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DemonstrationEvent();

		/**
		*  @brief
		*    Initialization
		*
		*  @param[in] demonstrationLayer
		*    Layer of entities in a map edited for this event
		*/
		void init(qsf::Layer* demonstrationLayer);

		/**
		*  @brief
		*    Get/Set properties
		*/
		float getDemonstrationRadius() const;
		void setDemonstrationRadius(float radius);
		uint32 getGangsterNumber() const;
		void setGangsterNumber(uint32 gangsterNumber);
		qsf::Time getTimeoutDelay() const;
		void setTimeoutDelay(qsf::Time timeout);
		uint32 getMaximalProtester() const;
		void setMaximalProtester(uint32 maximalProtester);

	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene(qsf::Layer& demonstrationLayer);
		void startHintObservers();
		void onShowHint01(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				mDemonstrationLayerId;
		std::vector<uint64> mCenterEntityIds;
		float				mDemonstrationRadius;
		uint32				mGangsterNumber;
		qsf::Time			mTimeoutDelay;
		uint32				mMaximalProtester;
		qsf::WeakPtr<qsf::Entity>	mSoundEntity;

		// internal
		uint32				mProtesterNumber;
		std::vector<bool>	mCenterTurnedViolent;	// TODO(co) Would a bitset be sufficient in here?
		qsf::MessageConfiguration mTimerMessage;
		qsf::MessageProxy	mDelayHint01TimerMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DemonstrationEvent)
