// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/sequence/SequenceEditorSelectionManager.h"	//< Included because nested types need to be accessible


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class SequenceDialog;
		class SequenceEditorWrapper;
		class SequenceOperationManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF sequence editor state manager
		*
		*  @remarks
		*    This is the main interface for changing/querying the state of the sequence editor.
		*    State changes can be subscribed to so you are notified of REAL state changes
		*    (changing the state to the same state won't fire any signals)
		*/
		class SequenceEditorState : public QObject
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Different states of the playhead
			*/
			enum PlaybackState
			{
				PS_PAUSE,
				PS_STOP,
				PS_PLAY_FORWARD,
				PS_PLAY_FORWARD_LOOP,
				PS_PLAY_BACKWARDS,
				PS_PLAY_BACKWARDS_LOOP,
				PS_PLAY_SECTION_FORWARD,
				PS_PLAY_SECTION_BACKWARD
			};


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		// Subscribe to them to get notified of state changes
		Q_SIGNALS:
			// Generic state signals
			void activeSequenceChanged(SequenceEditorWrapper* sequence);
			void timeSelectionChanged(float beginTime, float endTime);
			void timeChanged(float beginTime, float endTime);
			void playbackStateChanged(PlaybackState newPlaybackState);
			void playheadTimeChanged(float newPlayheadTime);
			void autokeyModeChanged(bool enabled);
			void keyTimeMoveModeChanged(bool enabled);
			void enabledEntityAdded(uint64 entityId);
			void keyMovementUpdated(); //< the key movement state has been updated

			// Sequence state signals
			void sequenceTimeRangeChanged(float newBeginTime, float newDuration);
			void sequenceTrackAdded(uint32 newTrackId);
			void sequenceTrackRemoved(uint32 trackId);

			// Track state signals
			void sequenceTrackKeyAdded(uint32 trackId, uint32 keyIndex);
			void sequenceTrackKeyChanged(uint32 trackId, uint32 keyIndex);
			void sequenceTrackKeyRemoved(uint32 trackId, uint32 keyIndex);


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Round time down to the nearest grid slot
			*/
			static float alignTimeToGrid(float time, float gridSize);

			/**
			*  @brief
			*    Round time to the nearest grid slot by the first decimal
			*/
			static float roundTimeToGrid(float time, float gridSize);

			/**
			*  @brief
			*    Rounds a position in time to a fixed frame number
			*/
			static int32 roundTimeToFrame(float time, float frameLength);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit SequenceEditorState(SequenceDialog& sequenceDialog);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SequenceEditorState();

			float getTimeSelectionBegin() const;
			float getTimeSelectionEnd() const;
			bool isAutokeyModeEnabled() const;
			bool isKeyTimeMoveModeEnabled() const;
			PlaybackState getPlaybackState() const;
			bool isPlaybackDisabled() const;
			float getPlayheadTime() const;
			uint32 getPlayheadFrame() const;
			SequenceEditorWrapper* getActiveSequence() const;
			SequenceOperationManager* getOperationManager() const;
			SequenceEditorSelectionManager& getSelectionManager() const;

			/**
			*  @brief
			*    Create an operation which atomically deletes all keys in the given set, also providing proper selection state change
			*
			*  @note
			*   - Do not chain combined operations because selection operations always are generated from the current state of selection
			*     thus the selection state change by multiple operations chained together will not work out well
			*/
			base::Operation* createMultiDeleteKeyOperation(const SequenceEditorSelectionManager::KeySelectionSet& keySelection);

			base::Operation* createMultiDeleteTrackOperation(const SequenceEditorSelectionManager::TrackSelectionSet& trackSelection);

			bool getIsMovingKeys() const;
			bool getIsMovingKeysCopying() const;
			int32 getKeyMovementTimeDifference() const;


		//[-------------------------------------------------------]
		//[ Public Qt slots (MOC)                                 ]
		//[-------------------------------------------------------]
		// Call these to change state
		public Q_SLOTS:
			// Generic state slots
			void setActiveSequence(SequenceEditorWrapper* sequence);
			void setTimeSelection(float beginTime, float endTime);
			void setPlayheadTime(float newTime);
			void setPlayheadFrameNumber(uint32 newFrameNumber);

			void reevaluateSequenceAtPlayhead();

			void setPlaybackState(PlaybackState newPlaybackState);
			void enableAutokeyMode(bool enable);
			void enableKeyTimeMoveMode(bool enable);

			void endKeyMovement();
			void updateKeyMovement(bool copying, int32 newFrameDifference);

			// Undo stack
			void sequenceEditingUndo();
			void sequenceEditingRedo();

			// Sequence state slots
			void setSequenceTimeRange(float beginTime, float duration);

			// Track state slots
			void notifySequenceTrackAdded(uint32 trackId);
			void notifySequenceTrackRemoved(uint32 trackId);
			void notifySequenceTrackKeyAdded(uint32 trackId, uint32 keyIndex);
			void notifySequenceTrackKeyChanged(uint32 trackId, uint32 keyIndex);
			void notifySequenceTrackKeyRemoved(uint32 trackId, uint32 keyIndex);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SequenceDialog&		   mSequenceDialog;
			SequenceEditorWrapper* mActiveSequence;
			float				   mTimelineSelectionBegin;
			float				   mTimelineSelectionEnd;
			bool				   mIsAutokeyModeEnabled;		//< Automatic key recording mode flag
			bool				   mIsKeyTimeMoveModeEnabled;	//< Moving of keys on the timeline enabled?

			PlaybackState	mPlaybackState;
			float			mPlayheadTime;
			float			mPlayheadTimeBeforePlaymode;

			SequenceOperationManager*		mOperationManager;	//< Our local undo stack
			SequenceEditorSelectionManager* mSelectionManager;	//< Manages the selection of tracks/keys

			bool  mIsMovingKeys;
			int32 mKeyMoveDifference;
			bool  mIsMovingKeysAndCopying;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
