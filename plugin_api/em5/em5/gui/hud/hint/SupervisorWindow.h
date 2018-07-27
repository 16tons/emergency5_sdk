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
#include <qsf/job/JobProxy.h>
#include <qsf/audio/AudioProxy.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class CameraTextureRenderComponent;
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
	*    Supervisor puppeteer
	*/
	class EM5_API_EXPORT SupervisorWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static float VOLUME_THRESHOLD;
		static float ANIMATION_FADE_TIME;

		enum State
		{
			STATE_FADEIN,
			STATE_1,
			STATE_SPEAK,
			STATE_2,
			STATE_FADEOUT,
			STATE_3,
			STATE_STOP
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit SupervisorWindow(qsf::GuiContext& guiContext);
		virtual ~SupervisorWindow();

		/**
		*  @brief
		*    Set whether or not a visible supervisor is allowed
		*/
		void setVisibleSupervisor(bool visible);

		/**
		*  @brief
		*    Play the audio and show the supervisor
		*
		*  @param[in] audioPath
		*    Asset path to the audio ressource
		*  @param[in] showVideo
		*    If we want to show the supervisor video or just have audio
		*/
		void play(const std::string& audioPath, bool showVideo);

		/**
		*  @brief
		*    Stop the audio and the supervisor
		*/
		void stop();

		/**
		*  @brief
		*    Fadeout the audio and the supervisor
		*/
		void fadeOut();

		/**
		*  @brief
		*    Return if a supervisor (audio or video) is running
		*/
		bool isRunning() const;

		void pause();

		void resume();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


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
		void instantUpdateTexture();
		void updateJob(const qsf::JobArguments& jobArguments);
		qsf::CameraTextureRenderComponent* getCameraTextureRenderComponent() const;
		void setCameraTextureRenderComponentActive(bool active);
		void setAnimation(bool enabled, bool speaking, bool blend);
		void setAudio(bool play);
		void onUpdateEvents(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				mVisibleSupervisor;
		State				mState;
		qsf::Map*			mMap;	///< Always valid
		uint64				mSupervisorEntityId;
		qsf::Time			mTimePassed;
		std::string			mAudioPath;
		qsf::JobProxy		mUpdateJobProxy;
		qsf::AudioProxy		mAudioProxy;
		uint32				mFreeplayEventId;
		qsf::MessageProxy	mMessageProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
