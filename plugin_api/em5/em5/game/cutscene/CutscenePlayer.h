// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/math/Color3.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class SequencePlayer;
}
namespace qsf
{
	class Entity;
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
	*    EMERGENCY 5 cutscene player
	*/
	class EM5_API_EXPORT CutscenePlayer : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum class SkipSetting
		{
			DISABLED,		///< No cutscene skipping allowed
			STOP,			///< Skipping immediately ends playback
			FASTFORWARD		///< Skipping fast-forwards playback with increased game speed (only for sequences, not videos)
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CutscenePlayer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CutscenePlayer();

		/**
		*  @brief
		*    Return the cutscene player' playback status
		*
		*  @return
		*    "true" if playback of a sequence (but not a screen fade) is currently active, "false" otherwise
		*
		*  @see
		*    - "em5::CutscenePlayer::isScreenFading()"
		*/
		bool isPlaying() const;

		/**
		*  @brief
		*    Start a cutscene with the given sequence
		*
		*  @param[in] sequenceLocalAssetName
		*    Local asset name of the sequence to load
		*  @param[in] movePlayerCamera
		*    If "true", the player camera gets moved with the sequence's camera, otherwise it stays where it was
		*  @param[in] allowVideo
		*    If "true", video usage is allowed, otherwise it's always realtime
		*
		*  @return
		*    "true" if playback has started, "false" on error like the sequence could not be loaded
		*/
		bool startCutscene(const std::string& sequenceLocalAssetName, bool movePlayerCamera, bool allowVideo = true, SkipSetting skipSetting = SkipSetting::STOP);

		/**
		*  @brief
		*    Stop the running cutscene, if there is one
		*/
		void stopCutscene(bool emitMessage);

		/**
		*  @brief
		*    Skip the running cutscene, i.e. jump to its end
		*/
		void skipCutscene();

		/**
		*  @brief
		*    Reaction to Esc key or similar
		*
		*  @return
		*    "true" in case Esc should not be further evaluated
		*/
		bool reactToEscape();

		/**
		*  @brief
		*    Show or hide the user interface
		*
		*  @param[in] show
		*    If "true", the user interface will be shown, otherwise it will be hidden
		*/
		void showUserInterface(bool show);

		/**
		*  @brief
		*    Return whether a screen fade is currently in progress
		*
		*  @return
		*    "true" if a screen fade is currently active, "false" otherwise
		*
		*  @see
		*    - "em5::CutscenePlayer::isPlaying()"
		*/
		bool isScreenFading() const;

		/**
		*  @brief
		*    Start a screen fading over time
		*
		*  @param[in] targetVisibility
		*    Screen visibility to fade to, between 0.0 for a totally faded-out scene and 1.0 for a fully visible scene
		*  @param[in] fadeLength
		*    Time to fade to the given target visibility
		*  @param[in] backgroundColor
		*    Color to show as background, usually "qsf::Color3::BLACK" or for a white-screen fading "qsf::Color3::WHITE"
		*  @param[in] endPause
		*    Optional additional pause at the end of the fading
		*
		*  @note
		*    - This will only affect the 3D scene, not the UI or debug output
		*/
		void startScreenFade(float targetVisibility, const qsf::Time& fadeLength, const qsf::Color3& backgroundColor = qsf::Color3::BLACK, const qsf::Time& endPause = qsf::Time::ZERO);
		void setScreenFade(float targetVisibility, const qsf::Color3& backgroundColor = qsf::Color3::BLACK);

		/**
		*  @brief
		*    Helper function to fly the camera to a certain camera entity
		*
		*  @notes
		*    - This is more for internal use; from event code, prefer using "startCameraFlight"
		*/
		bool flyCameraTo(qsf::Entity& targetCameraEntity, float maximumAcceleration = 100.0f, float maximumSpeed = 25.0f);

		/**
		*  @brief
		*    Start a camera flight
		*
		*  @notes
		*    - Emits the message "em5::Messages::EM5_CUTSCENE_CAMERA_FLIGHT_FINISHED" when camera flight is done
		*    - Blocks Esc menu during camera flight
		*/
		void startCameraFlight(qsf::Entity& targetCameraEntity, float maximumAcceleration = 100.0f, float maximumSpeed = 25.0f);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void checkJobUpdateRegistration();
		void setInputBlockedForCutscene(bool blocked);

		void movePlayheadToEnd(SequencePlayer& sequencePlayer);
		void feedTextureStreamer(const std::string& sequenceLocalAssetName);

		// Job update
		void updateRealtime(const qsf::JobArguments& jobArguments);

		// Message reactions
		void onSequenceEnded(const qsf::MessageParameters& messageParameters);
		void onSetCamera(const qsf::MessageParameters& messageParameters);
		void onEndVideo(const qsf::MessageParameters& messageParameters);
		void onCameraFlightStopped(const qsf::MessageParameters& parameters);

		void onScreenFadingDone();
		void showGui();

		void setTimeOfDayToSequence(bool isInSequenceUpdate);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		enum class Mode
		{
			NONE = 0,
			PLAYING_VIDEO,
			PLAYING_SEQUENCE,
			ONE_FRAME_DUMMY,
			SCREEN_FADE,		// Includes "end pause" time after actual screen fade
			CAMERA_FLIGHT
		};
		Mode				mCurrentMode;
		// TODO(fw): mCurrentMode was introduced after most variables below, so some of them are redundant now (e.g. mIsPlayingVideo, mDummyCutsceneRunning)

		bool				mIsPlayingVideo;				///< "true" if a replacement video is shown instead of a sequence
		bool				mIsChronologicCacheClearingEnabled;
		bool				mPreviousSpawnCivilActive;
		bool				mSilentMode;

		// Sequence playback
		SequencePlayer*		mSequencePlayer;				///< EMERGENCY 5 sequence player instance; may be a null pointer, we're responsible for destroying the instance
		bool				mMovePlayerCamera;				///< If "true", the player camera will adapt the sequence's camera transformation when the current sequence ends
		bool				mDummyCutsceneRunning;			///< If "true", a one-tick delay is currently running --- TODO(fw): Investigate whether this delay is still needed, or if there are better solutions
		SkipSetting			mSkipSetting;

		// Screen fade out effect
		float				mScreenVisibilityTarget;		///< Target value for screen visibility, between 0.0 (invisible) and 1.0 (fully visible)
		float				mScreenVisibilityChangeRate;	///< Change rate of screen visibility per second
		qsf::Time			mEndPause;						///< Optional additional pause at the end of the fading

		// Proxies
		qsf::JobProxy		mRealtimeUpdateJobProxy;
		qsf::MessageProxy	mEndSequenceMessageProxy;
		qsf::MessageProxy	mSetCameraMessageProxy;
		qsf::MessageProxy	mEndVideoMessageProxy;
		qsf::MessageProxy	mCameraFlightStoppedMessageProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CutscenePlayer)
