// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/gamemode/GameSessionConfiguration.h"

#include <qsf/job/JobProxy.h>
#include <qsf/gui/GuiDocument.h>
#include <qsf/input/InputListener.h>

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class ProgressBar;
}
namespace qsf
{
	class JobArguments;
	class CameraComponent;
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
	*    Medieval prelude screen
	*/
	class MedievalPreludeScreen : public qsf::GuiDocument, public qsf::InputListener
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void()>	PlaybackFinished;	///< This Boost signal is emitted when the prelude has been finished (or aborted premature)


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit MedievalPreludeScreen(qsf::GuiContext& guiContext, int gameSessionConfiguration = GameSessionConfiguration::EMERGENCY_2016_CAMPAIGN);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MedievalPreludeScreen();


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const qsf::InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void closeMedievalPreludeScreen();
		void updateAnimation(const qsf::JobArguments& jobArguments);
		void playAudioEffect();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_YEAR,
			STATE_STORY,
			STATE_TEXT,
			STATE_FADE_TO_BLACK,
			STATE_END
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ProgressBar*			mProgressBar;				///< Must be valid
		qsf::CameraComponent*	mCameraComponent;			///< Does not get destroyed while the Medieval prelude screen is shown, so a raw-pointer is safe, can be a null pointer
		float					mPreviousFpsLimitation;
		qsf::JobProxy			mAnimationJobProxy;			///< Regular animation update job proxy; for updates once a frame
		State					mState;
		qsf::Time				mTime;
		Rocket::Core::Element*	mElementAnnoDomini1327;
		Rocket::Core::Element*	mElementTheAgeOfPestilence;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
