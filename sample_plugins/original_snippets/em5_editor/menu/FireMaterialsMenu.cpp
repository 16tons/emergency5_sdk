// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/menu/FireMaterialsMenu.h"
#include "em5_editor/view/FireMaterialsView.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		FireMaterialsMenu::FireMaterialsMenu(bool fireMaterialSelected, FireMaterialsView& fireMaterialsView):
			mFireMaterialsView(fireMaterialsView)
		{
			// Setup the Qt actions
			{ // Add
				QAction* qAction = addAction(tr("ID_FIRE_MATERIAL_MENU_ADD"));
				connect(qAction, SIGNAL(triggered()), this, SLOT(onTriggeredAdd()));
			}

			{ // Duplicate
				QAction* qAction = addAction(tr("ID_FIRE_MATERIAL_MENU_DUPLICATE"));
				connect(qAction, SIGNAL(triggered()), this, SLOT(onTriggeredDuplicate()));
				qAction->setEnabled(fireMaterialSelected);
			}

			{ // Delete
				QAction* qAction = addAction(tr("ID_FIRE_MATERIAL_MENU_DELETE"));
				connect(qAction, SIGNAL(triggered()), this, SLOT(onTriggeredDelete()));
				qAction->setEnabled(fireMaterialSelected);
			}
		}

		FireMaterialsMenu::~FireMaterialsMenu()
		{
			// Nothing to do here
		}


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		void FireMaterialsMenu::onTriggeredDelete()
		{
			mFireMaterialsView.onClickedDelete(true);
		}

		void FireMaterialsMenu::onTriggeredAdd()
		{
			mFireMaterialsView.onClickedAdd(true);
		}

		void FireMaterialsMenu::onTriggeredDuplicate()
		{
			mFireMaterialsView.onClickedDuplicate(true);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
