// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline MainWindow& DialogManager::getMainWindow() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mMainWindow, "The QSF editor main window instance is invalid", QSF_REACT_THROW);
			return *mMainWindow;
		}

		template <typename T> int DialogManager::showModalDialog(QWidget* parent) const
		{
			// Get the dialog instance...
			T* dialog = get<T>();
			if (nullptr != dialog && !dialog->isVisible())
			{
				QWidget* oldParent = nullptr;

				if (nullptr != parent)
				{
					// Caller wants to set an specific parent
					// Store the old one ..
					oldParent = dialog->parentWidget();

					// .. set new parent and restore the current window flags, because the QWidget::setParent overloaded method clears any window flags
					dialog->setParent(parent, dialog->windowFlags());
				}

				// ... and show it as modal dialog
				int result = dialog->exec();

				// Restore old parent
				if (nullptr != oldParent)
				{
					dialog->setParent(oldParent, dialog->windowFlags());
				}

				return result;
			}

			// Error!
			return QDialog::Rejected;
		}

		template <typename T> void DialogManager::showDialog() const
		{
			// Get the dialog instance...
			T* dialog = get<T>();
			if (nullptr != dialog)
			{
				// ... and show it
				// -> Give it the focus, it appears that we need all three method calls (show, raise and activate) in order to ensure that this is always working correctly (Qt bug?)
				dialog->show();
				dialog->raise();
				dialog->activateWindow();
			}
		}

		template <typename T> void DialogManager::hideDialog() const
		{
			// Get the dialog instance...
			T* dialog = get<T>();
			if (nullptr != dialog && dialog->isVisible())
			{
				// ... and hide it
				dialog->close();
			}
		}

		template <typename T> void DialogManager::toggleDialogVisibility() const
		{
			// Get the dialog instance...
			T* dialog = get<T>();
			if (nullptr != dialog)
			{
				if (dialog->isVisible())
				{
					// Is visible, so close
					dialog->close();
				}
				else
				{
					// Is invisible, so show
					showDialog<T>();
				}
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
