// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/dialog/sequence/SequenceEditorSelectionManager.h"

#include <QtWidgets/qwidget.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class CompoundOperation;
		}
		class SequenceEditorTrackWrapper;
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
		*    TODO(co) Comment me
		*/
		class QKeyTimeline : public QWidget
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			static base::Operation* createMoveKeysOperation(SequenceEditorTrackWrapper& sequenceEditorTrackWrapper, const SequenceEditorSelectionManager::KeySelectionSet& keySelectionSet, int32 movementFramesOffset, bool isMovingKeyCopying, bool manualMove);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			QKeyTimeline(QWidget* parent, SequenceEditorTrackWrapper* track);
			bool getKeyFromPosition(QPointF localPos, uint32& outKeyIndex) const;
			float getTimeFromPosition(QPointF localPos) const;
			void contextMenuEvent(QContextMenuEvent* qContextMenuEvent);

			uint32 roundTimeToFrame(float freeTime) const;
			uint32 alignTimeToFrame(float freeTime) const;

			base::Operation* createMoveKeysOperation(const SequenceEditorSelectionManager::KeySelectionSet& keySelectionSet, int32 movementFramesOffset, bool isMovingKeyCopying, bool manualMove);


		//[-------------------------------------------------------]
		//[ Public Qt slots (MOC)                                 ]
		//[-------------------------------------------------------]
		public Q_SLOTS:
			void onTimeSelectionChanged(float newBeginTime, float newEndTime);
			void setSequenceTimeRange(float newTimeOrigin, float newDuration);
			void onKeyChangedAddedOrRemoved(uint32 trackId, uint32 keyIndex);
			void setPlayheadTime(float newTime);
			void notifySelectionChanged();
			void onKeyMovementUpdated();


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void mousePressEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent* qMouseEvent) override;
			virtual void paintEvent(QPaintEvent* paintEvent) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void selectKey(QMouseEvent& qMouseEvent, bool allowSelectionToggle);
			void drawKeyframe(QPainter& qPainter, uint32 frame, const QBrush& brushForFill, float keyframeLengthInPixels, bool isMovingKeyCopying = false) const;

			// TODO(oi) Maybe this can be moved into the track wrapper so everyone profits.
			void insertKeyAtFrameNumber(uint32 frameNumberAtCursor);

			void addCleanFramesBehind(uint32 frameWereToAdd, int32 framesToAdd);
			void removeFramesBehind(uint32 frameWereToRemove, int32 framesToRemove);

			void getKeySelectionSetBehindTime(SequenceEditorSelectionManager::KeySelectionSet& keySelectionSet, uint32 frameNumberWereToRemove);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SequenceEditorTrackWrapper*	mSequenceEditorTrackWrapper;
			uint32						mSelectedKeyIndex;
			bool						mHasSelection;
			bool						mIsDragging;

			float mSelectionBeginTime;
			float mSelectionEndTime;

			float mTimeOrigin;
			float mTimeEnd;

			float mPlayheadTime;

			uint32 mCurrentSelectedTrackId;
			bool   mIsCurrentSelectedTrackValid;

			enum eDragMode
			{
				DRAG_NONE,
				DRAG_KEY
			};

			eDragMode mCurrentDragMode;
			QPointF   mMouseDownPosition;
			uint32	  mDraggedKeyIndex;
			uint32	  mDraggedKeyOriginalFrameNumber;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
