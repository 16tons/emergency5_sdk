// Copyright (C) 2012-2018 Promotion Software GmbH


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
#include <qsf/map/Entity.h>


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
	*    EMERGENCY 5 freeplay event "Sabotage"
	*/
	class EM5_API_EXPORT SabotageEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::SabotageEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SabotageEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SabotageEvent();

		void init(qsf::Layer& eventLayer);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual bool onSuccess(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual void hintCallback(Observer& hintObserver) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives();
		bool getSubLayerFromMainLayer(qsf::Layer& mainLayer);
		void setupScene();

		void onDroneFoundGangster(const qsf::MessageParameters& parameters);

		void onBuriedFound(const qsf::MessageParameters& parameters);

		void pushMoveToEscapePoint(qsf::Entity& entity);
		qsf::Entity* getEscapePointById(int escapePointIndex);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Settings
		std::string mGangsterLogicName;

		// Setup
		uint32 mEventLayerId;

		// Internal
		qsf::Layer*	mEventLayer;
		qsf::Layer*	mEraseLayer;

		bool mIsGangsterHidden;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mCivilEscapePersons;
		std::vector<qsf::WeakPtr<qsf::Entity>>	mCivilEscapePoints;
		qsf::WeakPtr<qsf::Entity>	mStageEntity;
		qsf::WeakPtr<qsf::Entity>	mTweetCameraEntity;
		qsf::WeakPtr<qsf::Entity>	mTweetDebrisEntity;
		qsf::WeakPtr<qsf::Entity>	mTweetPersonEntity;
		bool	mCanTweet;
		bool	mHasTweeted;
		qsf::Time	mTimeTillTweet;	// Put a delay of one frame to the tweet to have a valid animation position
		int	mNumberPersonsTillTweet;
		std::vector<uint64>	mAllReadyFoundPersonsIds;

		struct StartEscaping
		{
			StartEscaping(qsf::Entity& entity, qsf::Time escapeTime) : mEscapeEntity(&entity), mTimeTillEscape(escapeTime), mStarted(false), mRemovedInvincible(false) { }

			bool mStarted;
			bool mRemovedInvincible;
			qsf::WeakPtr<qsf::Entity> mEscapeEntity;
			qsf::Time	mTimeTillEscape;
		};
		std::vector<StartEscaping>	mStartEscaping;

		qsf::MessageProxy	mGangsterFoundMessageProxy;
		qsf::MessageProxy	mBuriedFoundMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::SabotageEvent)
