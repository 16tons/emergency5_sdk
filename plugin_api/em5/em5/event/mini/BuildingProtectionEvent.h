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
#include <qsf/base/PropertyHelper.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class BlockAreaWithUnits;
	class WarblerTweetHelper;
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
	*    Emergency 2017 mini-event "Berlin Reichtag building protection"
	*/
	class EM5_API_EXPORT BuildingProtectionEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FreeplayEventMetaclasses2;		// CAMP export of editable values for Game Design


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::BuildingProtectionEvent" unique main event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BuildingProtectionEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BuildingProtectionEvent();


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
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupScene();
		void startObjectives();
		void startHintObservers();

		void onPoliceDogFoundObject(const qsf::MessageParameters& parameters);
		void onHiddenGangsterFound(const qsf::MessageParameters& parameters);

		void unlockTerrorCar();
		void spawnTerrorists();
		qsf::Entity& spawnTerrorist(qsf::Entity& spawnpoint);
		void letTerroristsFlee(uint64 alreadyFoundEntityId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		qsf::Time mBombTimer;
		qsf::Time mTimeToDefuseBomb;

		// Event entities
		qsf::WeakPtr<qsf::Entity> mEventArea;
		qsf::WeakPtr<qsf::Entity> mTerrorCar;
		qsf::WeakPtr<qsf::Entity> mBomb;
		std::vector<qsf::WeakPtr<qsf::Entity>> mTerroristSpawnPoints;
		std::vector<qsf::WeakPtr<qsf::Entity>> mTerrorists;
		std::vector<qsf::WeakPtr<qsf::Entity>> mTerroristWaypoints;

		struct GaperInfo
		{
			qsf::WeakPtr<qsf::Entity> mEntity;
			bool mInsideBox = false;
		};
		std::vector<GaperInfo> mGapers;

		BlockAreaWithUnits& mBlockAreaWithUnits;

		// Warbler tweet
		WarblerTweetHelper& mTweet01;

		// Event state
		enum class TerrorCarState
		{
			NO_INTERACTION,
			CAN_BE_INVESTIGATED,
			BOMB_PLACED
		};
		TerrorCarState mTerrorCarState;

		// Messages
		qsf::MessageProxy mDogFoundObjectMessageProxy;
		qsf::MessageProxy mVehicleCutOpenMessageProxy;
		qsf::MessageProxy mHiddenGangsterFoundMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::BuildingProtectionEvent)
