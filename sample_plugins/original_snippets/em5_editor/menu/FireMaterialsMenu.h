// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtWidgets/qmenu.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{
		class FireMaterialsView;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Fire materials menu
		*/
		class FireMaterialsMenu : public QMenu
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
			*    Constructor
			*
			*  @param[in] fireMaterialSelected
			*    "true" if the view select a valid material
			*  @param[in] fireMaterialsView
			*    Pointer to the fire materials view, must be valid
			*/
			FireMaterialsMenu(bool fireMaterialSelected, FireMaterialsView& fireMaterialsView);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~FireMaterialsMenu();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredDelete();
			void onTriggeredAdd();
			void onTriggeredDuplicate();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			FireMaterialsView& mFireMaterialsView;	///< Fire material view instance, do not destroy the instance


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
