// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qwidget.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class SequenceEditorWrapper;
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
		class QTimeZoomWidget : public QWidget
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
			/**
			*  @brief
			*    The selection time range was changed by this control
			*/
			void timeSelectionChanged(float beginTime, float endTime, bool isFinal);

			/**
			*  @brief
			*    Time of the playhead was changed manually
			*/
			void playheadTimeChanged(float oldTime, float newTime, bool isFinal);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit QTimeZoomWidget(QWidget* parent);


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void paintEvent(QPaintEvent* paintEvent) override;
			virtual void mousePressEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent* qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent* qMouseEvent) override;
			virtual void wheelEvent(QWheelEvent* qWheelEvent) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		public Q_SLOTS:
			/**
			*  @brief
			*    The viewed time range (time selection) has changed
			*/
			void setTimeSelection(float newBeginTime, float newEndTime);

			/**
			*  @brief
			*    The overall time range has changed
			*/
			void setSequenceTimeRange(float beginTime, float duration);

			void onPlayheadChanged(float newPlayheadTime);

			void setActiveSequence(SequenceEditorWrapper* sequence);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			float timeToPixels(float t) const;
			float selectionTimeToPixels(float t) const;
			float getPlayheadPixelPosition() const;
			float pixelsToPlayheadTime(float x) const;
			float pixelsToTime(float pixels) const;
			float pixelsToSelectionTime(float pixels) const;
			float getSelectionStartPixels() const;
			float getSelectionEndPixels() const;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			static const float HANDLE_TOLERANCE;

			enum DragHandle
			{
				DH_NONE,
				DH_BEGIN_TIME,
				DH_END_TIME,
				DH_COMPLETE_RANGE,
				DH_PLAYHEAD
			};


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			float mTimeOrigin;
			float mTimeEnd;

			float mSelectionTimeStart;
			float mSelectionTimeEnd;

			float mPlayheadPositionOriginal;
			float mPlayheadPosition;

			// Time where a drag operation has begun
			float mDragStartTime;
			float mDragStartDifferenceToStartTime;

			DragHandle mCurrentDraggedHandle;

			SequenceEditorWrapper* mActiveSequence;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
