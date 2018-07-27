// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qmenu.h>

#include "qsf_editor/application/manager/CameraManager.h"


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
		*    Camera tool bar menu
		*
		*  @note
		*    - This menu is more complex and dynamic, so it's justified to have a special menu class for this task
		*/
		class CameraToolBarMenu : public QMenu
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			CameraToolBarMenu();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CameraToolBarMenu();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredDuplicateCurrentCameraObject();
			void onTriggeredDeleteCurrentCameraObject();
			void onTriggeredLockedCamera();
			void onTriggeredFreeMoveCamera();
			void onTriggeredOrbitingCamera();
			void onTriggeredGameCamera();
			void onTriggeredTopDownCamera();
			void onTriggeredRenderViewCamera();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Set camera mode
			*
			*  @param[in] mode
			*    New camera mode to set
			*/
			void setCameraMode(CameraManager::CameraMode cameraMode);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
