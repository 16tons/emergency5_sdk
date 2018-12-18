// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/input/InputListener.h>
#include <qsf/renderer/component/CameraComponent.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class ElementText;
	}
}
namespace qsf
{
	class AudioMixer;
	class VideoPlayer;
	class JobArguments;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class EM5_API_EXPORT FullscreenVideoWindow : public qsf::GuiDocument, public qsf::InputListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit FullscreenVideoWindow(qsf::GuiContext& guiContext);
		virtual ~FullscreenVideoWindow();

		/**
		*  @brief
		*    Show video by filename
		*
		*  @param[in] videoFile
		*    Video file to play, for example "data/em5/local/video/logo/publisher_720p.ogv"
		*  @param[in] exclusiveMode
		*    "true" for exclusive mode resulting in that the video gets as much resources as possible by disabling some jobs, else "false"
		*/
		void showVideoByFilename(const std::string& videoFile, bool exclusiveMode = true);

		void clearVideo();
		bool isVideoPlaying() const;
		qsf::VideoPlayer& getVideoPlayer() const;


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
		void onPlaybackFinished();
		void onSyncVideo(const qsf::JobArguments& jobArgs);
		void disableJobManagerAutoupdate(const qsf::StringHash& providerJobManagerId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::AudioMixer&					mAudioMixer;
		std::shared_ptr<qsf::VideoPlayer>	mVideoPlayer;
		bool								mVideoPlaybackInProgress;
		float								mPreviousFpsLimitation;		///< The previous enforced FPS limitation, 0 if there's no limitation
		glm::ivec2							mScreenSize;
		std::vector<qsf::StringHash>		mDisabledJobManagerAutoupdate;
		qsf::WeakPtr<qsf::CameraComponent>	mPreviousMainCamera;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
