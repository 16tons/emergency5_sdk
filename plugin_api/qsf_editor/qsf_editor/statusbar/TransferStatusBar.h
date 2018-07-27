// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/statusbar/StatusBar.h"
#include "qsf_editor/asset/AssetStorageManagerTypes.h" // Needed for enum AssetTransferType because you cannot forward declare an enum (bad MS extension...)

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QProgressBar;
	class QHBoxLayout;
	class QLabel;
QT_END_NAMESPACE


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
		*    Transfer status bar
		*
		*  @note
		*    - The UI is created via source code
		*/
		class TransferStatusBar : public StatusBar
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::TransferStatusBar" unique pluginable status bar ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] statusBarManager
			*    Optional pointer to the status bar manager this status bar should be registered to, can be a null pointer
			*  @param[in] qWidgetParent
			*    Pointer to parent Qt widget, can be a null pointer (in this case you're responsible for destroying this status bar instance)
			*/
			TransferStatusBar(StatusBarManager* statusBarManager, QWidget* qWidgetParent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~TransferStatusBar();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::StatusBar methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void changeVisibility(bool visible) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateTransferLabels();


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onTransferStarted(AssetTransferType type, GlobalAssetId globalAssetId);
			void onTransferProgress(AssetTransferType type, GlobalAssetId globalAssetId, uint64 current, uint64 total);
			void onTransferCanceled(AssetTransferType type, GlobalAssetId globalAssetId);
			void onTransferFinished(AssetTransferType type, GlobalAssetId globalAssetId);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::pair<AssetTransferType, GlobalAssetId> TrackerKey;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::map<TrackerKey, QProgressBar*> mTracker;
			QLabel* mLabelEditorTransfer;
			QHBoxLayout* mLayoutEditorTransfer;
			QLabel* mLabelAssetTransfer;
			QHBoxLayout* mLayoutAssetTransfer;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TransferStatusBar)
