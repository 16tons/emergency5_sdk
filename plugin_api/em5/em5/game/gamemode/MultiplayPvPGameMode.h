// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/FreeplayGameMode.h"
#include "em5/network/multiplayer/OperationMessages.h"

#include <unordered_map>
#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		namespace server
		{
			class Host;
		}
	}
}
namespace qsf
{
	namespace game
	{
		class BitStream;
	}
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
	*    EMERGENCY 5 multiplay PVP game mode
	*/
	class EM5_API_EXPORT MultiplayPvPGameMode : public FreeplayGameMode
	{


	//[-------------------------------------------------------]
	//[ Public boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Called when a new event has arrived for selection
		*
		*  @note
		*    - Recommended slot signature: void onnewEventArrived(uint32 eventId, const std::string& eventName)
		*/
		boost::signals2::signal<void(uint32, const std::string&)> newEventArrived;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MultiplayPvPGameMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MultiplayPvPGameMode();


	//[-------------------------------------------------------]
	//[ Public virtual em5::GameMode methods                  ]
	//[-------------------------------------------------------]
	public:
		virtual std::string getGameModeAsString() const override;
		virtual void startup() override;
		virtual void updateGameMode(qsf::Time timePassed) override;
		virtual void onGameLoaded() override;
		virtual bool isSingleInteraction() const override;	// "true" means that only one player can interact with the event object


	//[-------------------------------------------------------]
	//[ Protected virtual em5::FreeplayGameMode methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onEventSucceeded(const qsf::MessageParameters& parameters) override;
		virtual void onEventFailed(const qsf::MessageParameters& parameters) override;
		virtual void onReceiveObjectivePoints(const qsf::MessageParameters& parameters) override;
		virtual void onSetPointGain(const FreeplayEvent::EventResult& eventResult, int32 pointGain) override;
		virtual void onFirstEventCompleted() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void onEventFinished(const FreeplayEvent::EventResult& eventResult, bool success);
		void onEventTriggered(const qsf::MessageParameters& parameters);
		void assignEventToPlayer(FreeplayEvent& freeplayEvent, uint32 playerIndex);
		bool anyEventSlotFree() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::Time												mRemainingTime;
		bool													mEndlessMode;
		multiplayer::server::Host*								mMultiplayerHost;
		std::unordered_map<uint32, std::unordered_set<uint32>>	mPerPlayerActiveEventCount;
		std::vector<uint32>										mPlayerListToChooseFrom;
		uint16													mMaxEventsPerPlayer;
		qsf::MessageProxy										mEventTriggeredMessageProxy;	///< Message proxy for the "freeplay event triggered" message


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MultiplayPvPGameMode)
