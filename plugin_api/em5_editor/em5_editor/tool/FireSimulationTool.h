// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <em5/fire/FireSystem.h>

#include <qsf_editor/tool/Tool.h>

#include <qsf/component/ComponentCollection.h>
#include <qsf/debug/DebugDrawProxy.h>

#include <QtCore/qobject.h>

#include <queue> // for an FIFO like container


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ui
{
	class FireSimulationTool;
}
namespace em5
{
	class FireComponent;
	class FireReceiverComponent;
	namespace editor
	{
		class FireComponentListModel;
	}
}
namespace qsf
{
	class Map;
	namespace editor
	{
		class EntitySelectionManager;
		class EditMode;
		namespace base
		{
			class Operation;
		}
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
		*    EMERGENCY 5 fire simulation class
		*
		*  @note
		*    - The UI is created by using a Qt ui-file (create/edit it by using e.g. Qt Designer)
		*/
		class FireSimulationTool : public QObject, public qsf::editor::Tool
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
			static const uint32 PLUGINABLE_ID;	///< "em5::editor::FireSimulationTool" unique pluginable tool ID


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
			explicit FireSimulationTool(qsf::editor::ToolManager* toolManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~FireSimulationTool();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(qsf::editor::ToolboxView& toolboxView) override;
			virtual void retranslateUi(qsf::editor::ToolboxView& toolboxView) override;
			virtual void onShutdown(qsf::editor::ToolboxView& toolboxView) override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			void onHideFireRadiansForAllItems(bool checked);
			void onShowHideFireRadiansForAllItems(bool checked);
			void onShowHideFireRadiansForSelectedItems(bool checked);

			void onShowHideFireChainsForSelectedItems(bool checked);

			void onStartSimulation(bool checked);
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const qsf::editor::base::Operation& operation);
			void onRedoOperationExecuted(const qsf::editor::base::Operation& operation);
			void onFireComponentListIndexChanged(const QModelIndex& current, const QModelIndex& previous);
			void onStartEditMode(bool checked);
			void onCreateEM5FireEntity();
			void onDeleteEM5FireEntity();
			void onEditModeChange(qsf::editor::EditMode* newEditMode, qsf::editor::EditMode* previousEditMode);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateUi();
			void connectDisconnectOperationSignals(bool disconnectSignals = false);
			void updateSelection(const qsf::editor::base::Operation& operation, bool isUndo);
			void changeShowFireRadiiForEntity(const qsf::Map& map, uint64 entityId, bool showRadii);
			void changeShowFireRadiiForAllItems(bool showRadii);
			void getFireComponentsFromEntity(const qsf::Map& map, uint64 entityId, std::vector<FireComponent*>& fireComponents) const;
			void getFireComponentsFromSelectedEntities(const qsf::Map& map, std::vector<FireComponent*>& fireComponents) const;
			void updateFireChainLines(const qsf::Map& map);
			// This method returns all fire receiver which a within the sphere which are represented by the center position and its radius
			bool getFireReceiverInSphere(const glm::vec3& center, float radius, const qsf::ComponentCollection::ComponentList<FireReceiverComponent>& allFireReceiverList, std::vector<FireReceiverComponent*>& outFireReceivers);
			void updateFireComponentListModel();
			// With this method all buttons gets enabled/disabled when switching the edit mode
			void enableDisableButtonsForEditMode(bool enabled);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QMetaObject::Connection					mUndoOperationConnection;		///< Holds the Qt signal/slot connection to the OperationManager::undoOperationExecuted signal
			QMetaObject::Connection					mRedoOperationConnection;		///< Holds the Qt signal/slot connection to the OperationManager::redoOperationExecuted signal
			Ui::FireSimulationTool*					mUiFireSimulationTool;			///< UI EM5 fire simulation tool instance, always valid, we have to destroy the instance in case we no longer need it
			qsf::editor::EntitySelectionManager&	mEntitySelectionManager;		///< Entity selection manager, do not destroy the instance
			FireSystem::SimulationVisualisationMode	mShowFireRadiiMode;				///< Visualization mode of the fire radii
			FireSystem::SimulationVisualisationMode	mShowFireChainsMode;			///< Visualization mode of the fire chains
			FireComponentListModel*					mFireComponentListModel;		///< View model for selected fire components
			uint64									mSelectedEntityIdForEditMode;	///< Holds the entity ID of the selected entity with a complex fire component when switching into the (complex) fire component edit mode
			bool									mInUndoRedoOperation;			///< Indicates if we are currently in an undo/redo operation
			qsf::DebugDrawProxy						mDebugDrawProxy;				///< Debug draw proxy for the fire chain lines


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::FireSimulationTool)
