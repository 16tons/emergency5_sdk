// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/view/EventPoolsView.h"
#include "ui_EventPoolsView.h"	// Automatically created by Qt's uic (output directory is "tmp\qt\uic\em5_editor" within the hand configured Visual Studio files, another directory when using CMake)

#include <qsf_editor/EditorHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		const uint32 EventPoolsView::PLUGINABLE_ID = qsf::StringHash("em5::editor::EventPoolsView");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		EventPoolsView::EventPoolsView(qsf::editor::ViewManager* viewManager, QWidget* qWidgetParent) :
			View(viewManager, qWidgetParent),
			mUiEventPoolsView(nullptr)
		{
			// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
			addViewAndTabify(reinterpret_cast<QMainWindow&>(*qWidgetParent), Qt::BottomDockWidgetArea);
		}

		EventPoolsView::~EventPoolsView()
		{
			// Destroy the UI event pools instance
			if (nullptr != mUiEventPoolsView)
			{
				delete mUiEventPoolsView;
			}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		void EventPoolsView::retranslateUi()
		{
			mUiEventPoolsView->retranslateUi(this);
		}

		bool EventPoolsView::preChangeVisibility(bool visible)
		{
			// TODO(co) Cooperative event pools editing is not supported, yet
			QSF_CHECK(!QSF_EDITOR_NETWORK_MANAGER.isLoggedIn(), "QSF editor: Cooperative event pools editing is not supported, yet", return false);

			// Show the dialog
			return visible;
		}

		void EventPoolsView::changeVisibility(bool visible)
		{
			// Lazy evaluation: If the view is shown the first time, create its content
			if (visible && nullptr == mUiEventPoolsView)
			{
				// Create and setup the view content
				// ... and here the magic happens...
				mUiEventPoolsView = new Ui::EventPoolsView();
				QWidget* qWidget = new QWidget(this);
				mUiEventPoolsView->setupUi(qWidget);
				setWidget(qWidget);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
