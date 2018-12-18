// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>

#include <QtWidgets/qwidget.h>

#include <boost/property_tree/ptree.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class SequenceEditorWrapper;
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
		class QCurveEditor : public QWidget
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			void setSelectedKey(bool valid, uint32 keyIndex);
			void timeSelectionChanged(float newBeginTime, float newEndTime);
			void playheadTimeChanged(float oldTime, float newTime, bool isFinal);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit QCurveEditor(QWidget* parentWidget);


		//[-------------------------------------------------------]
		//[ Public Qt slots (MOC)                                 ]
		//[-------------------------------------------------------]
		public Q_SLOTS:
			void setTimeSelection(float newBeginTime, float newEndTime);
			void setSequenceTimeRange(float newTimeOrigin, float newDuration);
			void updateTrackBounds();
			void setValueDisplayRange(float min, float max);	// Change the range of values displayed (y axis)
			void onPlayheadTimeChanged(float newPlayheadTime);
			void setActiveSequence(SequenceEditorWrapper* sequence);
			void notifySelectionChanged();

			// Track state slots
			void notifySequenceTrackKeyAdded(uint32 trackId, uint32 keyIndex);
			void notifySequenceTrackKeyChanged(uint32 trackId, uint32 keyIndex);
			void notifySequenceTrackKeyRemoved(uint32 trackId, uint32 keyIndex);

			/**
			*  @brief
			*    Does modify zoom values to focus the whole curve value range
			*/
			void focusWholeCurve();


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void mousePressEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent* qMouseEvent) override;
			virtual void wheelEvent(QWheelEvent* qWheelEvent) override;
			virtual void keyPressEvent(QKeyEvent * qKeyEvent) override;
			virtual void keyReleaseEvent(QKeyEvent * qKeyEvent) override;
			virtual void paintEvent(QPaintEvent* paintEvent) override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct BoundsTransform
			{
				static QPointF transform(QRectF source, QRectF target, QPointF inSource)
				{
					inSource.setX((inSource.x() - source.left()) / source.width());
					inSource.setY((inSource.y() - source.top()) / source.height());

					return QPointF(inSource.x() * target.width() + target.left(), inSource.y() * target.height() + target.top());
				}
			};

			struct KeyIndexWithSubtrackIndex
			{
				KeyIndexWithSubtrackIndex() :
					valid(true),
					keyIndex(0),
					subtrackIndex(0),
					tangent(false),
					incomingTangent(false)
				{}
				explicit KeyIndexWithSubtrackIndex(bool valid) :
					valid(valid),
					keyIndex(0),
					subtrackIndex(0),
					tangent(false),
					incomingTangent(false)
				{}

				bool valid;
				uint32 keyIndex;
				uint32 subtrackIndex;
				bool tangent;
				bool incomingTangent;

				bool operator == (const KeyIndexWithSubtrackIndex& other) const
				{
					if (other.valid == valid)
					{
						if (!valid)
						{
							return true;
						}

						return keyIndex == other.keyIndex &&
							subtrackIndex == other.subtrackIndex &&
							(tangent == other.tangent && (!tangent || (tangent && incomingTangent == other.incomingTangent)));
					}

					return false;
				}
			};

			// TODO(oi) remove magic number for invalid indices
			static const KeyIndexWithSubtrackIndex InvalidIndices;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			QRectF getTrackBounds() const;
			KeyIndexWithSubtrackIndex getKeySubtrackIndexFromPosition(QPointF localPosition, bool allowTangentsToBeGrabbed);

			/**
			*  @brief
			*    Will update the currently dragged key by re-applying the transformation from the last local mouse position; if no key is dragged, it's doing nothing
			*/
			void updateMovedKeyIfNeeded();
			void moveCurrentlyDraggedKeyToMouse(QPointF localMousePosition);

			void beginKeyEditing(KeyIndexWithSubtrackIndex keyData);
			void cancelKeyEditing();
			void finishKeyEditing();

			// TODO(oi) This code is mostly shared with QTimeZoomWidget methods, refactor into superclass!
			float timeToPixels(float t) const;
			float selectionTimeToPixels(float t) const;
			float getPlayheadPixelPosition() const;
			float pixelsToTime(float pixels) const;
			float frameToTime(uint32 frame);

			void zoomLevelToValueRange(float zoomLevel, float& valueRange);
			void valueRangeToZoomLevel(float valueRange, float& zoomLevel);

			/**
			*  @brief
			*    Modify the supplied values if the user is currently dragging around anything
			*/
			void modifyKeyDataFromEditOverride(uint32 keyIndex, uint subTrackIndex, float& curKeyT, float& curKeyV, float& curKeyCPIt, float& curKeyCPI, float& curKeyCPOt, float& curKeyCPO);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SequenceEditorTrackWrapper*		mEditedTrack;
			SequenceEditorWrapper*			mActiveSequence;

			QRectF			mTrackBounds;

			uint32			mSelectedKeyIndex;
			bool			mHasSelection;
			bool			mInvertKeyTimeMovementEnableFlag;
			QPointF			mDraggedKeyPosition;
			QPointF			mLastLocalMousePositionFromKeyMove;

			float mSelectionBeginTime;
			float mSelectionEndTime;

			float mSelectionBeginFrame;
			float mSelectionEndFrame;

			float mTimeOrigin;
			float mTimeEnd;

			float mZoomLevel;

			float mValueDisplayRangeMin;
			float mValueDisplayRangeMax;

			uint32 mPlayheadFrame;

			enum DragOperation
			{
				DRAG_NONE = 0,			///< No dragging in progress
				DRAG_PROBE_KEY = 1,		///< No drag yet, but potential drag has begun
				DRAG_KEY = 2,			///< Actively dragging a key
				DRAG_KEY_INCOMING = 3,	///< Drag the incoming control point for a key
				DRAG_KEY_OUTGOING = 4,	///< Drag the outgoing control point for a key
				DRAG_VIEW = 5,			///< Drag the view to scroll
				DRAG_PLAYHEAD_TIME = 6	///< Drag the playhead time
			};

			DragOperation				mCurrentDragOperation;
			KeyIndexWithSubtrackIndex	mCurrentlyDraggedKey;
			QPointF						mCurrentDragOrigin;
			float						mValueDisplayRangeMinOnDragStart;
			float						mValueDisplayRangeMaxOnDragStart;
			float						mTimeSelectionBeginOnDragStart;
			float						mTimeSelectionEndOnDragStart;

			boost::property_tree::ptree mSerializedKeyValueBeforeChange;
			bool						mIsEditingKey;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
