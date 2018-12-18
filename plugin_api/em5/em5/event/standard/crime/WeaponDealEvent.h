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
	*    EMERGENCY 5 freeplay event "weapon deal accident" (sometimes called drugdeal event)
	*/
	class EM5_API_EXPORT WeaponDealEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::WeaponDealEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		WeaponDealEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~WeaponDealEvent();

		void init(qsf::Layer& eventLayer);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GangsterTypeArmed, const std::string&, mGangsterTypeArmed)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(GangsterTypeUnarmed, const std::string&, mGangsterTypeUnarmed)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DetectPoliceRadius, float, mDetectPoliceRadius)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DeadPersonsToFail, int, mDeadPersonsToFail)


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
		void setupScene();
		void startPoliceArriving();
		void setupGangster(qsf::Entity& gangsterEntity, std::string gangsterLogicName);

		void onWeaponFound(const qsf::MessageParameters& parameters);
		void showEntityMesh(qsf::Entity& entity, bool active);
		void showKicklight(bool active);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Layer* mEventLayer;

		qsf::WeakPtr<qsf::Entity>	mGangsterCar;
		qsf::WeakPtr<qsf::Entity>	mWeaponObjectEntity;
		std::vector<qsf::WeakPtr<qsf::Entity>> mArmedGangsters;
		std::vector<qsf::WeakPtr<qsf::Entity>> mUnarmedGangsters;
		qsf::WeakPtr<qsf::Entity>	mTweetCameraEntity;
		qsf::WeakPtr<qsf::Entity>	mTweetPersonEntity;
		qsf::WeakPtr<qsf::Entity>	mTweetKickLightEntity;

		bool mCanMakeTweet;
		bool mCanTurnOffLight;
		qsf::Time mTweetDelay;

		// Settings
		std::string	mGangsterTypeArmed;
		std::string	mGangsterTypeUnarmed;
		float		mDetectPoliceRadius;
		int			mDeadPersonsToFail;

		// States
		bool	mPoliceArrived;	// Indicator for event started
		qsf::MessageProxy	mFoundWeaponMessageProxy;
		bool	mWeaponfound;
		bool	mHint01Played;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::WeaponDealEvent)
