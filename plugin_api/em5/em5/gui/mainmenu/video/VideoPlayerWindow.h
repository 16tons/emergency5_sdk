// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/gui/GuiDocument.h>
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
	class EM5_API_EXPORT VideoPlayerWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit VideoPlayerWindow(qsf::GuiContext& guiContext);
		virtual ~VideoPlayerWindow();

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

		bool isVideoPlaying() const;
		qsf::VideoPlayer& getVideoPlayer() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;
		virtual void processViewportDimensionsChanged(int width, int height) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGui();
		void onReloadGui();
		void clearVideo();
		void setupVideoImageElement(bool onlyPositionAndSize);
		void updateVideoTextureReference();
		void onPlaybackFinished();
		void onSyncVideo(const qsf::JobArguments& jobArguments);
		void disableJobManagerAutoupdate(const qsf::StringHash& providerJobManagerId);
		void updatePlayPauseButton();
		void updateProgressTime();
		void loadSubtitles();
		void clearSubtitles();
		void updateSubtitles();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct Subtitle
		{
			qsf::Time	startTime;
			qsf::Time	endTime;
			std::string	text;

			Subtitle(qsf::Time _startTime, qsf::Time _endTime, const std::string& _text) :
				startTime(_startTime),
				endTime(_endTime),
				text(_text)
			{
				// Nothing here
			}
		};
		typedef std::vector<Subtitle> Subtitles;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::AudioMixer&					mAudioMixer;
		std::unique_ptr<qsf::VideoPlayer>	mVideoPlayer;
		bool								mVideoPlaybackInProgress;
		qsf::Time							mTotalVideoDuration;
		float								mPreviousFpsLimitation;		///< The previous enforced FPS limitation, 0 if there's no limitation
		glm::ivec2							mScreenSize;
		std::vector<qsf::StringHash>		mDisabledJobManagerAutoupdate;
		qsf::WeakPtr<qsf::CameraComponent>	mPreviousMainCamera;
		std::string							mCurrentVideoFileName;
		Subtitles							mSubtitles;
		uint32								mCurrentSubtitleIndex;
		bool								mPreviousShowVideoSubtitles;
		bool								mVolumeSliderShown;
		Rocket::Core::Element*				mVideoElement;
		Rocket::Core::Element*				mVideoImageElement;
		Rocket::Core::ElementText*			mProgressTimeTextElement;
		Rocket::Core::Element*				mSubtitleElement;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
