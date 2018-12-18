// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/FreeplayGameMode.h"
#include "em5/map/MapIdentifier.h"


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
	*    EMERGENCY 5 challenge game mode
	*/
	class ChallengeGameMode : public FreeplayGameMode
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ChallengeGameMode();

		/**
		*  @brief
		*    Destructor
		*/
		~ChallengeGameMode();


	//[-------------------------------------------------------]
	//[ Public virtual em5::GameMode methods                  ]
	//[-------------------------------------------------------]
	public:
		virtual std::string getGameModeAsString() const override;
		virtual void startup() override;
		virtual void shutdown() override;
		virtual void updateGameMode(qsf::Time timePassed) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::FreeplayGameMode methods       ]
	//[-------------------------------------------------------]
	protected:
		virtual void onEventSucceeded(const qsf::MessageParameters& parameters) override;
		virtual void onEventFailed(const qsf::MessageParameters& parameters) override;
		virtual void onFirstEventCompleted() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onEventFinished(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		float	mEventDensityAbortDifference;
		bool	mFailed;
		uint32	mFailedEvents;
		uint32	mSuccessEvents;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ChallengeGameMode)
