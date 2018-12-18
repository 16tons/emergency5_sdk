// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/FreeplayGameMode.h"


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
	*    EMERGENCY 5 cooperative multiplay game mode
	*/
	class MultiplayCoopGameMode : public FreeplayGameMode
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MultiplayCoopGameMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MultiplayCoopGameMode();


	//[-------------------------------------------------------]
	//[ Public virtual em5::GameMode methods                  ]
	//[-------------------------------------------------------]
	public:
		virtual std::string getGameModeAsString() const override;
		virtual void startup() override;
		virtual void updateGameMode(qsf::Time timePassed) override;
		virtual void onGameLoaded() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::FreeplayGameMode methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onReceiveObjectivePoints(const qsf::MessageParameters& parameters) override;
		virtual void onSetPointGain(const FreeplayEvent::EventResult& eventResult, int32 pointGain) override;
		virtual void onFirstEventCompleted() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time					mRemainingTime;
		bool						mEndlessMode;
		multiplayer::server::Host*	mMultiplayerHost;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MultiplayCoopGameMode)
