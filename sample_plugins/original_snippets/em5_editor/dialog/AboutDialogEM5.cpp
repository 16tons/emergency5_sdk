// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/dialog/AboutDialogEM5.h"
#include "em5_editor/plugin/Plugin.h"
#include "ui_AboutDialogEM5.h"	// Automatically created by Qt's uic (output directory is "tmp\qt\uic\qsf_editor" within the hand configured Visual Studio files, another directory when using CMake)

#include <qsf_editor/application/MainWindow.h>

#include <qsf/QsfHelper.h>
#include <qsf/base/StringHash.h>
#include <qsf/file/FileSystem.h>
#include <qsf/plugin/QsfPlugin.h>
#include <qsf/plugin/Plugin.h>
#include <qsf/plugin/PluginSystem.h>
#include <qsf/plugin/version/PluginVersionBase.h>

#include <QtWidgets/qmenubar.h>
#include <QtGui/qdesktopservices.h>
#include <QtCore/qfile.h>
#include <QtCore/qtextstream.h>


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
		const uint32 AboutDialogEM5::PLUGINABLE_ID = qsf::StringHash("em5::editor::AboutDialogEM5");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		AboutDialogEM5::AboutDialogEM5(qsf::editor::DialogManager* dialogManager, QWidget* qWidgetParent) :
			AboutDialog(dialogManager, qWidgetParent),
			mUiAboutDialogEM5(nullptr)
		{
			// The EMERGENCY world builder should have another main application icon
			dialogManager->getMainWindow().setWindowIcon(QIcon(":/em5_editor.png"));
		}

		AboutDialogEM5::~AboutDialogEM5()
		{
			// Destroy the UI about dialog instance
			if (nullptr != mUiAboutDialogEM5)
			{
				delete mUiAboutDialogEM5;
			}
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::AboutDialog methods       ]
		//[-------------------------------------------------------]
		void AboutDialogEM5::createHelpMenu(QMenu& qMenu)
		{
			{ // Setup the open website Qt action
				QAction* qAction = new QAction(tr("ID_OPEN_WEBSITE"), this);
				connect(qAction, SIGNAL(triggered()), this, SLOT(onTriggeredMenuHelpWebsite()));
				qMenu.addAction(qAction);
			}

			{ // Setup the open Wiki Qt action
				QAction* qAction = new QAction(tr("ID_OPEN_WIKI"), this);
				connect(qAction, SIGNAL(triggered()), this, SLOT(onTriggeredMenuHelpWiki()));
				qMenu.addAction(qAction);
			}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Dialog methods         ]
		//[-------------------------------------------------------]
		void AboutDialogEM5::retranslateUi()
		{
			mUiAboutDialogEM5->retranslateUi(this);
		}

		void AboutDialogEM5::changeVisibility(bool visible)
		{
			// Lazy evaluation: If the dialog is shown the first time, create its content
			if (visible && nullptr == mUiAboutDialogEM5)
			{
				// ... and here the magic happens...
				mUiAboutDialogEM5 = new Ui::AboutDialogEM5();
				mUiAboutDialogEM5->setupUi(this);

				{ // Version information
					QListWidget* qListWidget = mUiAboutDialogEM5->listWidgetVersionInfo;

					// Add the list widget item
					const qsf::PluginVersionBase& qsfVersion = qsf::QsfPlugin::getInstanceSafe().getPluginVersion();
					const qsf::PluginVersionBase& em5Version = Plugin::getInstanceSafe().getPluginVersion();

					std::ostringstream stream;
					stream << "EMERGENCY WORLD BUILDER 2018 #" << em5Version.mBuildDate << " " << em5Version.mBuildTime << " (" << em5Version.mBuildConfiguration << " " << em5Version.mBuildArchitecture << ") " << "qsf #" << qsfVersion.mSvnRevision << " em5source #" << em5Version.mSvnRevision;

					// Add the list widget item
					QListWidgetItem* qListWidgetItem = new QListWidgetItem(QString::fromUtf8(stream.str().c_str()));
					qListWidget->addItem(qListWidgetItem);
				}

				{ // About text
					// Load the description from the about dialog
					QFile file(QString::fromStdString(QSF_FILE.getBaseDirectory() + "/data/em5/local/localization/en/about_dialog.html"));
					if (file.open(QIODevice::ReadOnly))
					{
						QTextStream in(&file);
						QString text;
						while (!in.atEnd())
						{
							text.append(in.readLine());
						}
						mUiAboutDialogEM5->plainTextEdit->appendHtml(text);
						mUiAboutDialogEM5->plainTextEdit->moveCursor(QTextCursor::Start);
					}
					file.close();
				}
			}
			mUiAboutDialogEM5->plainTextEdit->setReadOnly(true);
			mUiAboutDialogEM5->plainTextEdit->show();
		}


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		void AboutDialogEM5::onTriggeredMenuHelpWebsite()
		{
			QDesktopServices::openUrl(QUrl("http://em5.world-of-emergency.com/modding"));
		}

		void AboutDialogEM5::onTriggeredMenuHelpWiki()
		{
			QDesktopServices::openUrl(QUrl("http://www.emergency-forum.de/lexicon/index.php?lexicon/644-modding/&s=06e620272e1c58699632e71c3ef022108f8db53e"));
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
