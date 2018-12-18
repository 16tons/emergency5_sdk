// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/tool/Tool.h"
#include "qsf_editor/tool/ai/NavigationMapAssetChangeListener.h"
#include "qsf_editor/tool/ai/TrafficLightLockHelper.h"

#include <qsf/component/nodes/NodesComponent.h>

#include <QtCore/qobject.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class NavigationMapGeneratorTool;
}
namespace qsf
{
	namespace editor
	{
		class AssetEditHelper;
	}
	namespace ai
	{
		class TrafficLaneWorldCreationSettings;
	}
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
		*    Navigation map generator tool class
		*
		*  @note
		*    - Since the methods are called from Qt and we don't want to throw exceptions into Qt system calls, the tool acts as an exception firewall
		*    - Navigation Maps are called TrafficLaneWorld sometimes.
		*/
		class NavigationMapGeneratorTool : public QObject, public Tool
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
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::NavigationMapGeneratorTool" unique pluginable tool ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] toolManager
			*    The tool manager this tool instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit NavigationMapGeneratorTool(ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~NavigationMapGeneratorTool();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(ToolboxView& toolboxView) override;
			virtual void retranslateUi(ToolboxView& toolboxView) override;
			virtual void onShutdown(ToolboxView& toolboxView) override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			enum class AssetLockStatus
			{
				FAILED,
				SUCCESS,
				PENDING
			};


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onPressedAdd();
			void onPressedDelete();
			void onPressedCalculate();
			void onCurrentIndexChangedShow(int index);
			void onCellChanged(int row, int column);
			void onPressedDuplicate();
			void onSelectionChanged(int row, int column);
			void onCustomContextMenuRequested(const QPoint& qPoint);
			void onDataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight);
			void onPostMapRebuild();
			void onDelayedCall();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void rebuildGui();
			void updateDebugVisualization(bool toolVisible);
			void setAiAssetChangeListener();
			void onAssetReadyForStreetCalculation(GlobalAssetId globalAssetId, unsigned int currentSelectedWorldId);
			void onAssetReadyForAiCalculation(GlobalAssetId globalAssetId, unsigned int currentSelectedWorldId);
			void onAssetReadyForNameChange(GlobalAssetId globalAssetId, unsigned int currentSelectedWorldId, int row);
			void onAssetReadyForDeletion(GlobalAssetId globalAssetId, unsigned int currentSelectedWorldId);
			// TODO(sw) remove when AssetEditHelper support asset deletion
			void onAssetLockResultForDeletion(const QModelIndex qModelIndex, GlobalAssetId globalAssetId, bool isSuccess);
			void onChangedAssetEditDone();
			void showGettingLocksFailedErrorMessage();
			void showErrorMessage(const QString& title, const QString& message);
			// The mode doNotLockSelected is currently only needed when deleting an AI world map, because currently the AssetEditHelper doesn't support deletion of assets
			// Currently on delete we have to lock that asset separately
			void lockAiMapAssetsForEdit(const QModelIndex& qModelIndex, boost::function<void(GlobalAssetId, unsigned int currentSelectedWorldId)> readyCallback,bool lockOnlySelectedAsset = false, bool doNotLockSelected = false);
			void aiDataUpdateCompleted(bool success);

			void enableAllButtons(bool enabled);

			/**
			*  @brief
			*    Checks if all asset locks are successfully acquired
			*/
			AssetLockStatus checkIfAllLocksAreAcquired();

			/**
			*  @brief
			*    Refresh the content views
			*/
			void updateContent();
			void updateContentProperties();

			/**
			*  @brief
			*    Create new map
			*
			*  @param[in] wms
			*    Traffic lane world creation settings
			*/
			void createNewWorldMap(std::auto_ptr<ai::TrafficLaneWorldCreationSettings> wms, const std::string& fName = "");

			/**
			*  @brief
			*    Sort navigation maps for calculation by giving back a sorting index
			*/
			int navigationMapsSortingIndex(const QModelIndex& modelIndex);

			/**
			*  @brief
			*    Gets the next navigation map from our vector and locks the asset
			*/
			void lockNextAssetForCalculation();

			/**
			*  @brief
			*    Finishes calculation of navigation maps
			*/
			void finishCalculation();

			void updateMainMapAssetWritableRelatedButtons();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Ui::NavigationMapGeneratorTool*						mUiNavigationMapGeneratorTool;	///< UI navigation map generator tool instance, always valid, we have to destroy the instance in case we no longer need it
			std::auto_ptr<ai::TrafficLaneWorldCreationSettings> mWorldModelSettings;			///< Selected world model settings
			Object*												mInspectedObject;
			NavigationMapAssetChangeListener					mAiAssetChangeListener;			///< Provides callbacks for the AI manager to signal when it has updated an asset
			std::shared_ptr<qsf::editor::AssetEditHelper>		mAssetEditHelper;
			TrafficLightLockHelper								mLockedTrafficLightsHelper;
			std::vector<std::pair<QModelIndex, int>>			mMapsToCalculate;				///< Navigation maps we want to calculate
			int													mNumberOfMapsToCalculate;		///< Number of all Navigation maps we wanted to calculate


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::NavigationMapGeneratorTool)
