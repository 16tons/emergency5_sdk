// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/EditMode.h"
#include "qsf_editor/editmode/brush/BrushData.h"

#include <qsf/job/JobProxy.h>

#include <QtCore/qpoint.h>

#include <glm/glm.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QPoint;
QT_END_NAMESPACE
namespace qsf
{
	class JobArguments;
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
		*    Brush edit mode base class
		*/
		class BrushEditMode : public EditMode
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::BrushEditMode" unique pluginable edit mode ID

			/**
			*  @brief
			*    Brush input mode
			*/
			enum InputMode
			{
				BRUSHING_INPUT		  = 0,	///< Brushing input mode
				BRUSH_SIZE_INPUT	  = 1,	///< Change brush size input mode
				BRUSH_INTENSITY_INPUT = 2	///< Change brush intensity input mode
			};


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted after the brush size was changed
			*/
			void brushSizeChanged();

			/**
			*  @brief
			*    Qt signal emitted after the brush intensity was changed
			*/
			void brushIntensityChanged();


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BrushEditMode();

			/**
			*  @brief
			*    Fill brush data struct
			*
			*  @return
			*    The brush data
			*/
			const BrushData& getBrushData() const;

			/**
			*  @brief
			*    Set by brush data struct
			*
			*  @param[in] brushData
			*    The brush data
			*/
			void setBrushData(const BrushData& brushData);

			/**
			*  @brief
			*    Return the brush size
			*
			*  @return
			*    The brush size
			*/
			float getBrushSize() const;

			/**
			*  @brief
			*    Set the brush size
			*
			*  @param[in] brushSize
			*    The brush size
			*/
			void setBrushSize(float brushSize);

			/**
			*  @brief
			*    Return the brush intensity
			*
			*  @return
			*    The brush intensity
			*/
			float getBrushIntensity() const;

			/**
			*  @brief
			*    Set the brush intensity
			*
			*  @param[in] brushIntensity
			*    The brush intensity
			*/
			void setBrushIntensity(float brushIntensity);

			/**
			*  @brief
			*    Return the brush randomness
			*
			*  @return
			*    The brush randomness [0..1]
			*/
			float getBrushRandomness() const;

			/**
			*  @brief
			*    Set the brush randomness
			*
			*  @param[in] brushRandomness
			*    The brush randomness [0..1]
			*/
			void setBrushRandomness(float brushRandomness);

			/**
			*  @brief
			*    Return the brush position
			*
			*  @return
			*    The current world space position of the brush
			*/
			glm::vec3 getBrushPosition() const;

			/**
			*  @brief
			*    Set the brush position
			*
			*  @param[in] position
			*    The new world space position of the brush
			*/
			void setBrushPosition(const glm::vec3& position);

			/**
			*  @brief
			*    Return the brushing interval
			*
			*  @return
			*    The brushing interval in milliseconds
			*/
			int getBrushingInterval() const;

			/**
			*  @brief
			*    Set the brushing interval
			*
			*  @param[in] brushingInterval
			*    Brushing interval in milliseconds, "0" to disable brushing
			*
			*  @note
			*    - By default an brushing interval of 33 milliseconds resulting in 30 FPS is set
			*    - Do not set this brushing interval to low or it might cause Qt event processing issues
			*/
			void setBrushingInterval(int brushingInterval);

			/**
			*  @brief
			*    Return the brush type
			*
			*  @return
			*    The brush type
			*/
			BrushData::ShapeType getBrushType() const;

			/**
			*  @brief
			*    Set the brush type
			*
			*  @param[in] type
			*    The brush type
			*/
			void setBrushType(BrushData::ShapeType type);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit BrushEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Return whether or not brushing is currently enabled
			*
			*  @return
			*    "true" if brushing is currently enabled, else "false"
			*/
			bool isBrushingActive() const;

			/**
			*  @brief
			*    Enable/disable brushing
			*
			*  @param[in] active
			*    "true" to enable brushing, "false" to disable brushing
			*/
			void setBrushingActive(bool active);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::BrushEditMode methods  ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Evaluate the brush position
			*
			*  @param[in]  mousePosition
			*    Mouse position inside the render view to use for determining the world space position of the brush
			*  @param[out] position
			*    On success, receives the evaluated world space brush position, not touched on failure
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @note
			*    - The default implementation is using "qsf::RayMapQuery::getFirstHitByRenderWindow()" in order to evaluate the brush world space position
			*/
			virtual bool evaluateBrushPosition(const QPoint& mousePosition, glm::vec3& position);

			/**
			*  @brief
			*    Called as soon as brushing is started
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void onBrushingStartup();

			/**
			*  @brief
			*    Called each time a brush action should be executed
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void onBrushing();

			/**
			*  @brief
			*    Called as soon as brushing is stopped
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void onBrushingShutdown();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mousePressEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseReleaseEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;
			virtual void enterEvent(QEvent& qEvent) override;
			virtual void leaveEvent(QEvent& qEvent) override;
			virtual void keyPressEvent(QKeyEvent& qKeyEvent) override;
			virtual void keyReleaseEvent(QKeyEvent& qKeyEvent) override;


		//[-------------------------------------------------------]
		//[ Protected virtual QObject methods                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void timerEvent(QTimerEvent* qTimerEvent) override;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			InputMode					mInputMode;				///< Brush input mode
			bool						mSecondFunction;		///< "true" if shift-key is holding
			std::vector<unsigned int>	mDebugDrawRequestIds;	///< Debug draw request IDs


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Start the brush visualization
			*/
			void startBrushVisualization();

			/**
			*  @brief
			*    Update the brush visualization transform
			*/
			void updateBrushVisualizationTransform();

			/**
			*  @brief
			*    Stop the brush visualization
			*/
			void stopBrushVisualization();

			/**
			*  @brief
			*    Check if keys are released
			*
			*  @param[in] jobArguments
			*    Job arguments
			*/
			void keyCheckJob(const JobArguments& jobArguments);


		//[-------------------------------------------------------]
		//[ Private virtual qsf::editor::BrushEditMode methods    ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Build brush visualization shape
			*
			*  @note
			*    - The default implementation is empty
			*/
			virtual void buildBrushShape();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			glm::vec3	mBrushPosition;			///< The current world space position of the brush
			bool		mBrushingActive;		///< "true" if brushing is currently enabled, else "false"
			int			mBrushingInterval;		///< Brushing interval in milliseconds
			int			mBrushingQtTimerId;		///< Brushing Qt timer ID, "0" means no timer
			BrushData	mBrushData;				///< Additional Brushing Data
			JobProxy	mKeyCheckJobProxy;		///< Job who checks keys to ensure key release events are not forgotten


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::BrushEditMode)
