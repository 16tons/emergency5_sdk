// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/Export.h"

#include <qsf_editor/dialog/help/AboutDialog.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class AboutDialogEM5;
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
		*    EM5 editor about dialog
		*/
		class EM5EDITOR_API_EXPORT AboutDialogEM5 : public qsf::editor::AboutDialog
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
			static const uint32 PLUGINABLE_ID;	///< "em5::editor::AboutDialogEM5" unique pluginable dialog ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] dialogManager
			*    The dialog manager this dialog instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this dialog instance)
			*/
			AboutDialogEM5(qsf::editor::DialogManager* dialogManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~AboutDialogEM5();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::AboutDialog methods       ]
		//[-------------------------------------------------------]
		public:
			virtual void createHelpMenu(QMenu& qMenu) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		protected:
			virtual void retranslateUi() override;
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTriggeredMenuHelpWebsite();
			void onTriggeredMenuHelpWiki();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::AboutDialogEM5* mUiAboutDialogEM5;	///< UI about dialog instance, can be a null pointer, we have to destroy the instance in case we no longer need it


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::AboutDialogEM5)
