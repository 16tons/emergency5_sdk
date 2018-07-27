// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/QActions.h"


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
		*    Camera QActions category
		*/
		class CameraQActions : public QActions
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::CameraQActions" unique pluginable QActions ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] qActionsManager
			*    The QtActions manager this QtActions instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit CameraQActions(QActionsManager* qActionsManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CameraQActions();

			/**
			*  @brief
			*    Return the change camera view Qt action
			*
			*  @return
			*    The change camera view Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getChangeCameraViewQAction() const;

			/**
			*  @brief
			*    Return the select current camera object Qt action
			*
			*  @return
			*    The select current camera object Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getSelectCurrentCameraObjectQAction() const;

			/**
			*  @brief
			*    Return the capture screenshot Qt action
			*
			*  @return
			*    The capture screenshot Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCaptureScreenshotQAction() const;

			/**
			*  @brief
			*    Return the capture cube screenshot Qt action
			*
			*  @return
			*    The capture cube screenshot Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getCaptureCubeScreenshotQAction() const;

			/**
			*  @brief
			*    Return the fly to position Qt action
			*
			*  @return
			*    The fly to position Qt action, can be a null pointer, do not destroy the returned instance
			*/
			inline QAction* getFlyToPositionQAction() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::QActions methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onChangeCameraView();
			void onSelectCurrentCameraObject();
			void onTriggeredCaptureScreenshot();
			void onTriggeredCaptureCubeScreenshot();
			void onTriggeredFlyToPosition();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QAction* mChangeCameraViewQAction;			///< Change camera view Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mSelectCurrentCameraObjectQAction;	///< Select current camera object Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mCaptureScreenshotQAction;			///< Capture screenshot Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mCaptureCubeScreenshotQAction;		///< Capture cube screenshot Qt action instance, can be a null pointer, Qt will automatically destroy the instance
			QAction* mFlyToPositionQAction;				///< Fly to position Qt action instance, can be a null pointer, Qt will automatically destroy the instance


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/qaction/renderer/CameraQActions-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::CameraQActions)
