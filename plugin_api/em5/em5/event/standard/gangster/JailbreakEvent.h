// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/standard/gangster/GangsterEvent.h"

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
	*    EMERGENCY 5 freeplay event class for "jailbreak"
	*/
	class EM5_API_EXPORT JailbreakEvent : public GangsterEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::JailbreakEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		JailbreakEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~JailbreakEvent();

		std::vector<uint64>& getPrisonerIds();

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ArmedChance, float, mArmedChance);


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
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startHintObservers();
		void onGameTimerMessage(const qsf::MessageParameters& parameters);

		void setupGangster(qsf::Entity& gangsterEntity);

		void onPrisonerFound(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		float				mArmedChance;

		// Internal
		std::vector<uint64>	mPrisonerIds;
		qsf::Time			mPrisonersSeekTime;
		bool				mAllGangsterDetectByDrone;

		std::vector<qsf::MessageProxy>	mPrisonerFoundMessageProxies;
		qsf::MessageProxy				mGameTimerMessageProxy;

		qsf::Time				mHint01WaitTime;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::JailbreakEvent)
