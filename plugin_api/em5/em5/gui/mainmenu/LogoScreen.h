// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>
#include <qsf/input/InputListener.h>
#include <qsf/job/JobProxy.h>
#include <qsf/time/Time.h>


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
	*    EMERGENCY 5 logo screen
	*/
	class LogoScreen : public qsf::GuiDocument, public qsf::InputListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit LogoScreen(qsf::GuiContext& guiContext);
		virtual ~LogoScreen();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;
		virtual void processViewportDimensionsChanged(int width, int height) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const qsf::InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateJob(const qsf::JobArguments&);
		void setupVideo(const std::string& filename, Rocket::Core::Element* videoPlaneElement);
		void updateVideoRect(Rocket::Core::Element* videoPlaneElement);
		void skipOneLogo();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_FIRST_LOGO,
			STATE_FIRST_LOGO_PLAY,
			STATE_FIRST_DELAY,
			STATE_SECOND_LOGO,
			STATE_SECOND_LOGO_PLAY,
			STATE_SECOND_DELAY,
			STATE_THIRD_LOGO,
			STATE_THIRD_PLAY,
			STATE_THIRD_DELAY,
			STATE_END
		};
		State mState;

		uint32 mWidth;
		uint32 mHeight;
		float  mGuiVolume;

		qsf::JobProxy		mUpdateJobProxy;
		qsf::Time			mTime;
		qsf::VideoPlayer*	mVideoPlayer;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
