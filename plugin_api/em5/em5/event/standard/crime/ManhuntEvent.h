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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class WarblerTweetHelper;
}
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
	*    EMERGENCY 5 freeplay event "Manhunt"
	*/
	class EM5_API_EXPORT ManhuntEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::ManhuntEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Calculates the event tag for the houses from the layer name
		*/
		static const std::string getHouseEventagFromEventLayerName(const std::string& layerName);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ManhuntEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ManhuntEvent();

		void init(qsf::Layer& eventLayer);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TimeTillEventLose, qsf::Time, mTimeTillEventLose);


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

		void setupGangsterHouses();
		void setupGangsterHouse(qsf::Entity& houseEntity, bool isFakeObject);
		void removeRandomFakeBuilding();
		void removeFakeBuildingIcons(qsf::Entity& houseEntity);
		void checkFalseHouseEmpty();

		void onSpeakToPerson(const qsf::MessageParameters& parameters);
		void onAttackBuilding(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Layer* mEventLayer;
		qsf::Layer* mEventSceneryLayer;
		qsf::Layer* mEventEraseLayer;

		std::vector<qsf::WeakPtr<qsf::Entity>> mAttestors;		// Persons the player has to talk
		std::vector<qsf::WeakPtr<qsf::Entity>> mFalseHouses;	// Houses with false flag
		qsf::WeakPtr<qsf::Entity> mGangsterHouse;

		// Settings
		qsf::Time	mTimeTillEventLose;

		// States
		bool	mCanSetupGangsterHouses;	// Indicator for event started
		bool	mFoundRightHouse;
		qsf::MessageProxy	mSpeakToPersonMessageProxy;
		qsf::MessageProxy	mAttackBuildingMessageProxy;
		qsf::MessageProxy	mAttackBuildingFakeMessageProxy;
		bool	mCanTriggerHint02;	// Hint for attacking false house

		WarblerTweetHelper& mWarblerTweetHelper;	// To support the delayed hints, we need an instance of the warbler tweet helper (for the update)

		std::vector<uint64>	mAttackedBuildings;
		int	mNumberAttackedFalseHouses;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ManhuntEvent)
