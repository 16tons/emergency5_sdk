// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/mainmenu/MainMenuWindowBase.h"

#include <qsf/job/JobProxy.h>

#include <Rocket/Core/EventListener.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class VideoPlayer;
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
	*    Credits window
	*/
	class CreditsWindow : public MainMenuWindowBase, public Rocket::Core::EventListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit CreditsWindow(qsf::GuiContext& guiContext);
		virtual ~CreditsWindow();
		void showEndScreen(const std::string& endScreenFilename);


	//[-------------------------------------------------------]
	//[ Public virtual Rocket::Core::EventListener methods    ]
	//[-------------------------------------------------------]
	public:
		virtual void ProcessEvent(Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::MainMenuWindowBase methods     ]
	//[-------------------------------------------------------]
	protected:
		virtual void onGoingBack() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GuiHotReloadSupport methods    ]
	//[-------------------------------------------------------]
	protected:
		virtual void onReloadGui() override;
		virtual bool onDocumentPreShow() override;
		virtual void onDocumentPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGui();
		void hideEndScreen(const qsf::JobArguments& jobArguments);
		void moveText(float offset) const;
		void updateJob(const qsf::JobArguments& jobArguments);
		void onVideoPlaybackFinished();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum class FadeState
		{
			NO_FADING,
			FADE_IN,
			FADE_OUT
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy		   mJobProxy;
		std::string			   mEndScreenFilename;
		qsf::JobProxy		   mEndScreenJobProxy;
		qsf::VideoPlayer*	   mVideoPlayer;
		float				   mMusicMixerChannel;
		float				   mVideoMixerChannel;
		Rocket::Core::Element* mBackgroundImageElement;
		FadeState			   mFadeState;
		float				   mFadeFactor;	///< 1 = credits are fully shown, 0 = credits are not shown


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
