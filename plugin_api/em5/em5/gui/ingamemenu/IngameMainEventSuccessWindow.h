// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/main/MainEvent.h"
#include "em5/gui/GuiHotReloadSupport.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/job/JobProxy.h>
#include <qsf/audio/AudioProxy.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	enum AnimationState {
		ShowTitle,
		ShowCompleted,
		ShowEfficiency,
		ShowScore,
		CountScore,
		ShowHighscore,
		None
	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    IngameMainEventSuccessWindow window
	*
	*  @remarks
	*    Before displaying this window the concept describes in 3.3.3 a pause mode with a frozen screen
	*    pointing to the reason for the failed game.
	*/
	class IngameMainEventSuccessWindow : public GuiHotReloadSupport
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit IngameMainEventSuccessWindow(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~IngameMainEventSuccessWindow();

		/**
		*  @brief
		*    Set the results for the event
		*  @todo
		*    We need to save personal scores for every user and every event
		*/
		void setEventResults(MainEvent::EventResult& eventResults, uint32 mainEventNumber, uint32 silver, uint32 gold);

		void setMainEvent(MainEvent* mainEvent);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GuiHotReloadSupport methods    ]
	//[-------------------------------------------------------]
	protected:
		virtual void onReloadGui() override;
		virtual bool onDocumentPreShow() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onShowWindow();

		/**
		*  @brief
		*    Upload scores function for the results
		*/
		void publish(bool state);

		void setPlayerControl(bool playerControl) const;

		void cleanupAfterEvent();

		/**
		*  @brief
		*    Updates the animations
		*/
		void onUpdateAnimation(const qsf::JobArguments& jobArguments);

		void setupGui();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy			mAnimationJobProxy;	///< Regular animation update job proxy; for updates once a frame
		MainEvent*				mMainEvent;			///< Holds a pointer to the main event, which has shown this dialog, can be a nullptr
		AnimationState			mAnimationState;	///< holds the current animation state
		float					mPastTime;			///< Holds the past time while the animation is running
		uint32					mTotalScore;		///< Hold the overall score that the user had reached
		uint32					mCountingScore;		///< Counter for the counting animation
		uint32					mMainEventNumber;	///< Holds the main event number
		qsf::AudioProxy			mAudioProxy;
		qsf::AudioProxy			mAudioProxyMedal;
		bool					mPlayMedalSound;
		bool					mLastMainEvent;		///< Is this the campaign's last main event?


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
