// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/tool/FireSimulationTool.h"
#include "em5_editor/model/FireComponentListModel.h"
#include "em5_editor/editmode/FireComponentEditMode.h"
#include "ui_FireSimulationTool.h"	// Automatically created by Qt's uic (output directory is "tmp\qt\uic\em5_editor" within the hand configured Visual Studio files, another directory when using CMake)

#include <em5/fire/component/FireComponent.h>
#include <em5/fire/component/ComplexFireComponent.h>

#include <qsf_editor/editmode/map/ObjectEditMode.h>
#include <qsf_editor/operation/entity/EntityOperationHelper.h>
#include <qsf_editor/operation/entity/SelectEntityOperation.h>
#include <qsf_editor/selection/entity/EntitySelectionManager.h>
#include <qsf_editor/view/utility/ToolboxView.h>
#include <qsf_editor/EditorHelper.h>

#include <qsf_editor_base/operation/entity/CreateEntityOperation.h>
#include <qsf_editor_base/operation/entity/AddEntitiesToSelectionOperation.h>
#include <qsf_editor_base/operation/entity/RemoveEntitiesFromSelectionOperation.h>
#include <qsf_editor_base/operation/entity/SelectEntityHiddenOperation.h>
#include <qsf_editor_base/operation/entity/DeselectEntityHiddenOperation.h>
#include <qsf_editor_base/operation/CompoundOperation.h>
#include <qsf_editor_base/operation/component/SetComponentPropertyOperation.h>
#include <qsf_editor_base/operation/component/CreateComponentOperation.h>
#include <qsf_editor_base/user/User.h>

#include <qsf/application/ApplicationContext.h>
#include <qsf/prototype/helper/SlotHelper.h>
#include <qsf/debug/request/CircleDebugDrawRequest.h>
#include <qsf/debug/request/SegmentDebugDrawRequest.h>
#include <qsf/component/base/TransformComponent.h>
#include <qsf/component/base/MetadataComponent.h>
#include <qsf/component/link/LinkComponent.h>
#include <qsf/math/CoordinateSystem.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Qt metatype                                           ]
//[-------------------------------------------------------]
Q_DECLARE_METATYPE(em5::FireComponent*)


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{
		namespace detail
		{
			struct FireChainData
			{
				glm::vec3	mStartPosition;	///< Start position of the line
				glm::vec3	mEndPosition;	///< End position of the line
				qsf::Color4	mLineColor;		///< Line color
				// Need an default constructor otherwise vector::emplace_back(<val>, <val2>) doesn't work...
				FireChainData() {}

				// Need an constructor otherwise vector::emplace_back(<val>, <val2>) doesn't work...
				FireChainData(const glm::vec3& startPosition, const glm::vec3& endPosition, const qsf::Color4& lineColor) :
					mStartPosition(startPosition),
					mEndPosition(endPosition),
					mLineColor(lineColor)
				{
				}
			};

			class WaitCursor
			{
				public:
					explicit WaitCursor(const QCursor& cursor)
					{
						QApplication::setOverrideCursor(cursor);
					}

					~WaitCursor()
					{
						QApplication::restoreOverrideCursor();
					}
			};
		}


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		const uint32 FireSimulationTool::PLUGINABLE_ID = qsf::StringHash("em5::editor::FireSimulationTool");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		FireSimulationTool::FireSimulationTool(qsf::editor::ToolManager* toolManager) :
			Tool(toolManager),
			mUiFireSimulationTool(new Ui::FireSimulationTool()),
			mEntitySelectionManager(QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>()),
			mShowFireRadiiMode(FireSystem::SimulationVisualisationMode::SHOW_NONE),
			mShowFireChainsMode(FireSystem::SimulationVisualisationMode::SHOW_NONE),
			mFireComponentListModel(nullptr),
			mSelectedEntityIdForEditMode(qsf::getUninitialized<uint64>()),
			mInUndoRedoOperation(false)
		{
			// Nothing to do in here
		}

		FireSimulationTool::~FireSimulationTool()
		{
			// Destroy the UI EM5 fire simulation tool instance
			delete mUiFireSimulationTool;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::Tool methods           ]
		//[-------------------------------------------------------]
		bool FireSimulationTool::onStartup(qsf::editor::ToolboxView& toolboxView)
		{
			// ... and here the magic happens...
			mUiFireSimulationTool->setupUi(toolboxView.widget());

			// Initial the edit mode only buttons are disabled and the change edit mode button is unchecked
			mUiFireSimulationTool->pushButtonChangeMode->setChecked(false);
			mUiFireSimulationTool->pushButtonCreateEM5Fire->setEnabled(false);
			mUiFireSimulationTool->pushButtonDeleteEM5Fire->setEnabled(false);

			// Connect Qt signals/slots
			connect(mUiFireSimulationTool->pushButtonHideFireRadiansForAllItems, SIGNAL(toggled(bool)), this, SLOT(onHideFireRadiansForAllItems(bool)));
			connect(mUiFireSimulationTool->pushButtonShowHideFireRadiansForAllItems, SIGNAL(toggled(bool)), this, SLOT(onShowHideFireRadiansForAllItems(bool)));
			connect(mUiFireSimulationTool->pushButtonShowFireRadiansForSelectedItems, SIGNAL(toggled(bool)), this, SLOT(onShowHideFireRadiansForSelectedItems(bool)));

			connect(mUiFireSimulationTool->pushButtonShowFireChainsForSelectedItems, &QPushButton::toggled, this, &FireSimulationTool::onShowHideFireChainsForSelectedItems);

			connect(mUiFireSimulationTool->pushButtonStartSimulation, SIGNAL(clicked(bool)), this, SLOT(onStartSimulation(bool)));
			connect(mUiFireSimulationTool->pushButtonChangeMode, SIGNAL(toggled(bool)), this, SLOT(onStartEditMode(bool)));
			connect(mUiFireSimulationTool->pushButtonCreateEM5Fire, SIGNAL(clicked()), this, SLOT(onCreateEM5FireEntity()));
			connect(mUiFireSimulationTool->pushButtonDeleteEM5Fire, SIGNAL(clicked()), this, SLOT(onDeleteEM5FireEntity()));

			connectDisconnectOperationSignals();

			mFireComponentListModel = new FireComponentListModel(mUiFireSimulationTool->fireComponentListView);
			mUiFireSimulationTool->fireComponentListView->setModel(mFireComponentListModel);

			connect( mUiFireSimulationTool->fireComponentListView->selectionModel(), SIGNAL( currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT( onFireComponentListIndexChanged(const QModelIndex&, const QModelIndex&) ) );

			updateFireComponentListModel();

			updateUi();

			// Done
			return true;
		}

		void FireSimulationTool::retranslateUi(qsf::editor::ToolboxView& toolboxView)
		{
			mUiFireSimulationTool->retranslateUi(toolboxView.widget());
		}

		void FireSimulationTool::onShutdown(qsf::editor::ToolboxView& toolboxView)
		{
			connectDisconnectOperationSignals(true);

			disconnect(&QSF_EDITOR_EDITMODE_MANAGER, &qsf::editor::EditModeManager::change, this, &FireSimulationTool::onEditModeChange);

			// Leave our editmode when active
			qsf::editor::EditModeManager& editModeManager = QSF_EDITOR_EDITMODE_MANAGER;
			if (editModeManager.getSelectedEditMode() == editModeManager.get<FireComponentEditMode>())
			{
				editModeManager.selectEditModeByPointer(editModeManager.getPreviousEditMode(), editModeManager.getToolWhichSelectedEditMode());
			}

			// Hide all fire radii and fire chain lines
			changeShowFireRadiiForAllItems(false);

			// Unregister debug draw proxy for the fire chain lines
			mDebugDrawProxy.unregister();
		}


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		void FireSimulationTool::onHideFireRadiansForAllItems(bool checked)
		{
			if (checked)
			{
				mShowFireRadiiMode = FireSystem::SimulationVisualisationMode::SHOW_NONE;
				changeShowFireRadiiForAllItems(false);
			}
		}

		void FireSimulationTool::onShowHideFireRadiansForAllItems(bool checked)
		{
			if (checked)
			{
				mShowFireRadiiMode = FireSystem::SimulationVisualisationMode::SHOW_GLOBAL;
				changeShowFireRadiiForAllItems(true);
			}
		}

		void FireSimulationTool::onShowHideFireRadiansForSelectedItems(bool checked)
		{
			mShowFireRadiiMode = checked ? FireSystem::SimulationVisualisationMode::SHOW_ONLY_SELECTED : FireSystem::SimulationVisualisationMode::SHOW_NONE;

			if (checked)
			{
				changeShowFireRadiiForAllItems(false);

				const qsf::editor::EntitySelectionManager::IdSet& selectedIds = mEntitySelectionManager.getSelectedIdSet();
				const qsf::Map& map = QSF_MAINMAP;

				for (uint64 entityId : selectedIds)
				{
					changeShowFireRadiiForEntity(map, entityId, true);
				}
			}
		}

		void FireSimulationTool::onShowHideFireChainsForSelectedItems(bool checked)
		{
			mShowFireChainsMode = checked ? FireSystem::SimulationVisualisationMode::SHOW_ONLY_SELECTED : FireSystem::SimulationVisualisationMode::SHOW_NONE;

			if (checked)
			{
				updateFireChainLines(QSF_MAINMAP);
			}
			else
			{
				mDebugDrawProxy.unregister();
			}
		}

		void FireSimulationTool::onStartSimulation(bool checked)
		{
			boost::property_tree::ptree parametersPTree;
			parametersPTree.put("Mode", "FireSpreadPreview");

			// Save a backup of the map
			QSF_EDITOR_APPLICATION.storeMapBackup();

			// Switch to another QSF application context, return to the QSF editor application when done
			qsf::Qsf::instance()->switchApplicationContext(*(new qsf::ApplicationContext("../game.app.json", parametersPTree)), true);
		}

		void FireSimulationTool::onUndoOperationExecuted(const qsf::editor::base::Operation& operation)
		{
			// Ignore operations from other users
			// Because this tool handles only operations for the current user
			// Operations caused by the current user have not userId set -> value is qsf::getUninitialized<uint32>()
			if (qsf::isInitialized(operation.getUserId()))
			{
				return;
			}

			mInUndoRedoOperation = true;

			updateSelection(operation, true);
			disconnect( mUiFireSimulationTool->fireComponentListView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onFireComponentListIndexChanged(const QModelIndex&, const QModelIndex&)));
			mFireComponentListModel->onUndoOperationExecuted(operation);
			connect( mUiFireSimulationTool->fireComponentListView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onFireComponentListIndexChanged(const QModelIndex&, const QModelIndex&)));

			mInUndoRedoOperation = false;
		}

		void FireSimulationTool::onRedoOperationExecuted(const qsf::editor::base::Operation& operation)
		{
			// Ignore operations from other users
			// Because this tool handles only operations for the current user
			// Operations caused by the current user have not userId set -> value is qsf::getUninitialized<uint32>()
			if (qsf::isInitialized(operation.getUserId()))
			{
				return;
			}

			mInUndoRedoOperation = true;

			updateSelection(operation, false);
			disconnect( mUiFireSimulationTool->fireComponentListView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onFireComponentListIndexChanged(const QModelIndex&, const QModelIndex&)));
			mFireComponentListModel->onRedoOperationExecuted(operation);
			connect( mUiFireSimulationTool->fireComponentListView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onFireComponentListIndexChanged(const QModelIndex&, const QModelIndex&)));

			mInUndoRedoOperation = false;
		}

		void FireSimulationTool::onFireComponentListIndexChanged(const QModelIndex& current, const QModelIndex& previous)
		{
			// Using here pointers to FireComponents is save, because we have the lock onto the parent entity which contains the fire component
			// -> The component cannot be deleted by anyone else except ourself.
			FireComponent* selectedItem = current.isValid() ? current.data(Qt::UserRole).value<FireComponent*>() : nullptr;
			FireComponent* previousSelectedItem = previous.isValid() ? previous.data(Qt::UserRole).value<FireComponent*>() : nullptr;

			mUiFireSimulationTool->fireComponentPropertiesView->setInspectedObject(selectedItem);

			if (mShowFireRadiiMode == FireSystem::SimulationVisualisationMode::SHOW_NONE)
			{
				if (previousSelectedItem)
				{
					previousSelectedItem->setShowFireRadii(false);
				}
				if (selectedItem)
				{
					selectedItem->setShowFireRadii(true);

					const qsf::editor::EditModeManager& editModeManager = QSF_EDITOR_EDITMODE_MANAGER;
					if (!mInUndoRedoOperation && editModeManager.getSelectedEditMode() == editModeManager.get<FireComponentEditMode>())
					{
						QSF_EDITOR_OPERATION.push(mEntitySelectionManager.createSelectEntityOperation(selectedItem->getEntityId()));
					}
				}
			}
		}

		void FireSimulationTool::onStartEditMode(bool checked)
		{
			if (checked)
			{
				// Try to add a additional lock to the entity because we are going to change the selection -> the lock would be released
				qsf::editor::base::SelectEntityHiddenOperation* selectEntityHiddenOperation = QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>().createSelectEntityHiddenOperation(mEntitySelectionManager.getCurrentId());
				QSF_ASSERT(nullptr != selectEntityHiddenOperation, "Could not lock selected entity", return);

				// Commit the operation
				QSF_EDITOR_OPERATION.push(selectEntityHiddenOperation);

				mSelectedEntityIdForEditMode = mEntitySelectionManager.getCurrentId();

				// Change into fire component edit mode
				QSF_EDITOR_EDITMODE_MANAGER.selectEditMode<FireComponentEditMode>();

				connect(&QSF_EDITOR_EDITMODE_MANAGER, &qsf::editor::EditModeManager::change, this, &FireSimulationTool::onEditModeChange);

				const FireComponent* fireComponent = static_cast<FireComponent*>(mUiFireSimulationTool->fireComponentPropertiesView->getInspectedObject());
				if (nullptr != fireComponent)
				{
					QSF_EDITOR_OPERATION.push(mEntitySelectionManager.createSelectEntityOperation(fireComponent->getEntityId()));
				}

				enableDisableButtonsForEditMode(false);
			}
			else
			{
				// Leave our editmode when active
				qsf::editor::EditModeManager& editModeManager = QSF_EDITOR_EDITMODE_MANAGER;
				if (editModeManager.getSelectedEditMode() == editModeManager.get<FireComponentEditMode>())
				{
					// Save the entity ID of the main entity -> the member gets unset after the edit mode is left
					const uint64 mainEntity = mSelectedEntityIdForEditMode;

					editModeManager.selectEditModeByPointer(editModeManager.getPreviousEditMode(), editModeManager.getToolWhichSelectedEditMode());

					// Select the main entity again
					QSF_EDITOR_OPERATION.push(QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>().createSelectEntityOperation(mainEntity));
				}
			}
			updateUi();
		}

		void FireSimulationTool::onCreateEM5FireEntity()
		{
			if (qsf::isUninitialized(mSelectedEntityIdForEditMode))
				return;

			qsf::Entity* selectedEntity = QSF_MAINMAP.getEntityById(mSelectedEntityIdForEditMode);
			if (nullptr == selectedEntity)
				return;
			qsf::TransformComponent* transformComponent = selectedEntity->getComponent<qsf::TransformComponent>();
			if (nullptr == transformComponent)
				return;

			// Get ID of the new entity and the layer to which the entity should belong
			const uint64 entityId = QSF_MAINMAP.generatePrototypeId();
			const int layerId = selectedEntity->getComponentSafe<qsf::MetadataComponent>().getLayerId();

			// Create entity with EM5 fire component and link it to the selected entity
			qsf::editor::base::CompoundOperation* compoundOperation = new qsf::editor::base::CompoundOperation();

			// Create the create entity operation
			compoundOperation->pushBackOperation(new qsf::editor::base::CreateEntityOperation(entityId, layerId));

			const std::string entityName = "fire_entity" + std::to_string(entityId);

			// Change the name of the object in metadata component
			compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(entityId, qsf::MetadataComponent::COMPONENT_ID, qsf::MetadataComponent::NAME, entityName));

			// Change the layer ID
			compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(entityId, qsf::MetadataComponent::COMPONENT_ID, qsf::MetadataComponent::LAYER_ID, layerId));

			// Build an operation that adds the transform component
			compoundOperation->pushBackOperation(new qsf::editor::base::CreateComponentOperation(entityId, qsf::TransformComponent::COMPONENT_ID));
			compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(entityId, qsf::TransformComponent::COMPONENT_ID, qsf::TransformComponent::POSITION, transformComponent->getPosition()));

			// Build an operation that adds the fire component
			compoundOperation->pushBackOperation(new qsf::editor::base::CreateComponentOperation(entityId, FireComponent::COMPONENT_ID));

			// Add link component
			compoundOperation->pushBackOperation(new qsf::editor::base::CreateComponentOperation(entityId, qsf::LinkComponent::COMPONENT_ID));

			// Setup link component
			compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(entityId, qsf::LinkComponent::COMPONENT_ID, qsf::LinkComponent::PARENT_LINK_TYPE, qsf::LinkComponent::HARD_LINK));
			compoundOperation->pushBackOperation(new qsf::editor::base::SetComponentPropertyOperation(entityId, qsf::LinkComponent::COMPONENT_ID, qsf::LinkComponent::PARENT_ID, selectedEntity->getId()));

			// Update fire entities
			ComplexFireComponent* complexFireComponent = selectedEntity->getComponent<ComplexFireComponent>();
			qsf::SlotHelper::SlotIdentifier slotIdentifier(*complexFireComponent, *complexFireComponent->getCampPropertyById(ComplexFireComponent::FIREENTITIES_ARRAY));
			compoundOperation->pushBackOperation(qsf::editor::EntityOperationHelper::builAddEntityToParentSlot(*selectedEntity, entityId, slotIdentifier));

			// Select this entity
			compoundOperation->pushBackOperation(QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>().createSelectEntityOperation(entityId));

			// Set text for compound operation
			compoundOperation->setText("Create " + entityName);

			// Execute the operation
			QSF_EDITOR_OPERATION.push(compoundOperation);
		}

		void FireSimulationTool::onDeleteEM5FireEntity()
		{
			QModelIndex currentIndex = mUiFireSimulationTool->fireComponentListView->currentIndex();
			if (currentIndex.isValid())
			{
				qsf::Entity* selectedEntity = QSF_MAINMAP.getEntityById(mSelectedEntityIdForEditMode);
				if (nullptr == selectedEntity)
					return;

				const FireComponent* selectedItem = currentIndex.data(Qt::UserRole).value<FireComponent*>();

				mUiFireSimulationTool->fireComponentListView->clearSelection();

				qsf::editor::base::CompoundOperation* compoundOperation = static_cast<qsf::editor::base::CompoundOperation*>(qsf::editor::EntityOperationHelper::buildDestroyEntityOperation(selectedItem->getEntity(), false));

				// Update fire entities
				// Build slot identifier
				ComplexFireComponent* complexFireComponent = selectedEntity->getComponent<ComplexFireComponent>();
				qsf::SlotHelper::SlotIdentifier slotIdentifier(*complexFireComponent, *complexFireComponent->getCampPropertyById(ComplexFireComponent::FIREENTITIES_ARRAY));
				compoundOperation->pushBackOperation(qsf::editor::EntityOperationHelper::buildRemoveEntityFromParentSlot(*selectedEntity, selectedItem->getEntityId(), slotIdentifier));
				QSF_EDITOR_OPERATION.push(compoundOperation);
			}
		}

		void FireSimulationTool::onEditModeChange(qsf::editor::EditMode* newEditMode, qsf::editor::EditMode* previousEditMode)
		{
			if (QSF_EDITOR_EDITMODE_MANAGER.get<qsf::editor::ObjectEditMode>() == newEditMode && QSF_EDITOR_EDITMODE_MANAGER.get<FireComponentEditMode>() == previousEditMode)
			{
				// We left the fire component edit mode
				// -> All selections are gone

				// Unregister Qt signal for edit mode change
				disconnect(&QSF_EDITOR_EDITMODE_MANAGER, &qsf::editor::EditModeManager::change, this, &FireSimulationTool::onEditModeChange);

				// Unlock the selected entity
				QSF_EDITOR_OPERATION.push(QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>().createDeselectEntityHiddenOperation(mSelectedEntityIdForEditMode));

				mSelectedEntityIdForEditMode = qsf::getUninitialized<uint64>();

				enableDisableButtonsForEditMode(true);

				mUiFireSimulationTool->pushButtonChangeMode->setChecked(false);
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void FireSimulationTool::updateUi()
		{
			// Enable/disable the button for switching to the fire entity edit mode
			// The button gets only enabled when
			// - Only one entity is selected
			// - The selected entity has a complex fire component
			const qsf::editor::EditModeManager& editModeManager = QSF_EDITOR_EDITMODE_MANAGER;
			const qsf::editor::EntitySelectionManager::IdSet& selectedIds = mEntitySelectionManager.getSelectedIdSet();

			if (editModeManager.getSelectedEditMode() != editModeManager.get<FireComponentEditMode>() && selectedIds.size() == 1)
			{
				const qsf::Entity* selectedEntity = QSF_MAINMAP.getEntityById(*selectedIds.begin());
				if (nullptr != selectedEntity)
				{
					mUiFireSimulationTool->pushButtonChangeMode->setEnabled(selectedEntity->getComponent<ComplexFireComponent>() != nullptr);
				}
			}
			else if (editModeManager.getSelectedEditMode() == editModeManager.get<FireComponentEditMode>())
			{
				mUiFireSimulationTool->pushButtonChangeMode->setEnabled(true);
			}
			else
			{
				mUiFireSimulationTool->pushButtonChangeMode->setEnabled(false);
			}
		}

		void FireSimulationTool::connectDisconnectOperationSignals(bool disconnectSignals)
		{
			if (disconnectSignals)
			{
				disconnect(mUndoOperationConnection);
				disconnect(mRedoOperationConnection);
			}
			else
			{
				if (!mUndoOperationConnection)
				{
					mUndoOperationConnection = connect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::undoOperationExecuted, this, &FireSimulationTool::onUndoOperationExecuted);
				}
				if (!mRedoOperationConnection)
				{
					mRedoOperationConnection = connect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::redoOperationExecuted, this, &FireSimulationTool::onRedoOperationExecuted);
				}
			}
		}

		void FireSimulationTool::updateSelection(const qsf::editor::base::Operation& operation, bool isUndo)
		{
			// TODO(sw) handle create/destroy entity/component when visualisation mode is show global?
			const uint32 operationId = operation.getId();
			if (qsf::editor::base::SelectEntityOperation::OPERATION_ID == operationId)
			{
				// A single entity was selected
				const qsf::Map& map = QSF_MAINMAP;

				if (mShowFireRadiiMode == FireSystem::SimulationVisualisationMode::SHOW_ONLY_SELECTED)
				{
					// Update the fire radii status for all previous selected entities when they had a fire components
					// When we are in a undo operation then the previous selected entities draw their radii
					// Otherwise the selected entity draw its radii
					const qsf::editor::base::SelectEntityOperation& entitySelectionOperation = static_cast<const qsf::editor::base::SelectEntityOperation&>(operation);
					for (uint64 entityId : entitySelectionOperation.getPreviousEntityIdSet())
					{
						changeShowFireRadiiForEntity(map, entityId, isUndo);
					}

					// Update the fire radii status for the selected entity if it have a fire component
					changeShowFireRadiiForEntity(map, entitySelectionOperation.getEntityId(), !isUndo);
				}

				if (mShowFireChainsMode != FireSystem::SimulationVisualisationMode::SHOW_NONE)
				{
					// Update fire chains for the selected entities when they have a fire component
					updateFireChainLines(map);
				}

				// Update the fire component list model
				updateFireComponentListModel();
			}
			else if (qsf::editor::base::AddEntitiesToSelectionOperation::OPERATION_ID == operationId)
			{
				// Multiple entities are selected or added

				const qsf::Map& map = QSF_MAINMAP;
				if (mShowFireRadiiMode == FireSystem::SimulationVisualisationMode::SHOW_ONLY_SELECTED)
				{
					// Update the fire radii status for all (newly) selected entities when they have a fire components
					// When we are in a undo operation then the (newly) selected entities hide their radii
					// Otherwise the (newly) selected entities draw their radii

					const qsf::editor::base::AddEntitiesToSelectionOperation& entitySelectionOperation = static_cast<const qsf::editor::base::AddEntitiesToSelectionOperation&>(operation);
					const qsf::editor::EntitySelectionManager::IdSet& selectedIds = entitySelectionOperation.getEntityIdSet();
					for (uint64 entityId : selectedIds)
					{
						changeShowFireRadiiForEntity(map, entityId, !isUndo);
					}
				}

				if (mShowFireChainsMode != FireSystem::SimulationVisualisationMode::SHOW_NONE)
				{
					// Update fire chains for the selected entities when they have a fire component
					updateFireChainLines(map);
				}

				// Update the fire component list model
				updateFireComponentListModel();
			}
			else if (qsf::editor::base::RemoveEntitiesFromSelectionOperation::OPERATION_ID == operationId)
			{
				// Entities got unselected
				const qsf::Map& map = QSF_MAINMAP;
				if (mShowFireRadiiMode == FireSystem::SimulationVisualisationMode::SHOW_ONLY_SELECTED)
				{
					// Update the fire radii status for all unselected entities when they have a fire components
					// When we are in a undo operation then the unselected entities draw their radii
					// Otherwise the (newly) selected entities hide their radii

					const qsf::editor::base::RemoveEntitiesFromSelectionOperation& entitySelectionOperation = static_cast<const qsf::editor::base::RemoveEntitiesFromSelectionOperation&>(operation);
					const qsf::editor::EntitySelectionManager::IdSet& selectedIds = entitySelectionOperation.getEntityIdSet();
					for (uint64 entityId : selectedIds)
					{
						changeShowFireRadiiForEntity(map, entityId, isUndo);
					}
				}

				if (mShowFireChainsMode != FireSystem::SimulationVisualisationMode::SHOW_NONE)
				{
					// Update fire chains for the selected entities when they have a fire component
					updateFireChainLines(map);
				}

				// Update the fire component list model
				updateFireComponentListModel();
			}

			// Update the enabled status of the button for switching to the fire entity edit mode
			updateUi();
		}

		void FireSimulationTool::changeShowFireRadiiForEntity(const qsf::Map& map, const uint64 entityId, bool showRadii)
		{
			std::vector<FireComponent*> fireComponents;
			getFireComponentsFromEntity(map, entityId, fireComponents);

			for (FireComponent* fireComponent : fireComponents)
			{
				fireComponent->setShowFireRadii(showRadii);
			}
		}

		void FireSimulationTool::changeShowFireRadiiForAllItems(bool showRadii)
		{
			for (FireComponent* fireComponent : qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<FireComponent>())
			{
				fireComponent->setShowFireRadii(showRadii);
			}
		}

		void FireSimulationTool::getFireComponentsFromEntity(const qsf::Map& map, uint64 entityId, std::vector<FireComponent*>& fireComponents) const
		{
			const qsf::Entity* entity = map.getEntityById(entityId);
			if (nullptr == entity)
			{
				return;
			}

			FireComponent* fireComponent = entity->getComponent<FireComponent>();
			if (nullptr != fireComponent)
			{
				fireComponents.push_back(fireComponent);
			}
			else
			{
				ComplexFireComponent* complexFireComponet = entity->getComponent<ComplexFireComponent>();
				if (nullptr != complexFireComponet)
				{
					complexFireComponet->getAllFireComponents(fireComponents);
				}
			}
		}

		void FireSimulationTool::getFireComponentsFromSelectedEntities(const qsf::Map& map, std::vector<FireComponent*>& fireComponents) const
		{
			for (uint64 entityId : mEntitySelectionManager.getSelectedIdSet())
			{
				getFireComponentsFromEntity(map, entityId, fireComponents);
			}
		}

		void FireSimulationTool::updateFireChainLines(const qsf::Map& map)
		{
			// This calculation can take some time -> show an busy cursor
			QCursor cursor(Qt::WaitCursor);
			detail::WaitCursor waitCursor(cursor);

			// Reset old debug requests, we will generate new one here
			mDebugDrawProxy.registerAt(map.getDebugDrawManager());

			// We use a FIFO like container,because while processing one fire component new fire components might be found to be also processed
			// (the new components are located inside the soft radius of the processed fire component)
			std::queue<FireComponent*> fireComponentsList;

			// Get all fire components from the selected items
			for (uint64 entityId : mEntitySelectionManager.getSelectedIdSet())
			{
				std::vector<FireComponent*> fireComponents;
				getFireComponentsFromEntity(map, entityId, fireComponents);

				for (FireComponent* fireComponent : fireComponents)
				{
					qsf::TransformComponent* transformComponent = fireComponent->getTransformComponent();
					if (nullptr != transformComponent)
					{
						fireComponentsList.push(fireComponent);
					}
				}
			}

			if (fireComponentsList.empty())
			{
				return;
			}

			std::vector<detail::FireChainData> fireChainDataList;
			std::vector<glm::vec3> fireComponentPostionCircles;

			// Get all fire receiver components (also fire components)
			const qsf::ComponentCollection::ComponentList<FireReceiverComponent>& allFireReceiverList = qsf::ComponentMapQuery(map).getAllInstances<FireReceiverComponent>();

			// Process next fire component until the list is empty
			// While processing an component new components might be added to the list
			while (!fireComponentsList.empty())
			{
				// Get the next fire component and remove it from the list
				FireComponent* fireComponent = fireComponentsList.front();
				fireComponentsList.pop();

				const qsf::TransformComponent* transformComponent = fireComponent->getTransformComponent();
				if (nullptr != transformComponent)
				{
					const glm::vec3& sourcePosition = transformComponent->getPosition();

					// Get all fire receivers which are inside the soft radius around the processed fire component
					std::vector<FireReceiverComponent*> fireComponents;
					getFireReceiverInSphere(sourcePosition, fireComponent->getSoftRadius(), allFireReceiverList, fireComponents);

					for (FireReceiverComponent* fireReceiverComponent : fireComponents)
					{
						// The result list holds also the processed component -> ignore it
						if (fireComponent == fireReceiverComponent)
						{
							continue;
						}

						if (fireReceiverComponent->getId() == FireComponent::COMPONENT_ID)
						{
							// Add the position of the fire receiver to the list when the position isn't already added
							// Around these positions an circle is drawn
							if (std::find(fireComponentPostionCircles.cbegin(), fireComponentPostionCircles.cend(), sourcePosition) == fireComponentPostionCircles.cend())
							{
								fireComponentPostionCircles.push_back(sourcePosition);
							}
						}

						const qsf::TransformComponent* targetTransformComponent = fireReceiverComponent->getTransformComponent();
						if (nullptr != targetTransformComponent)
						{
							// Check if we will draw already a line between the two component
							// Only one line between two positions is drawn
							const glm::vec3& targetPosition = targetTransformComponent->getPosition();
							auto endIterator = fireChainDataList.end();
							auto iterator = std::find_if(fireChainDataList.begin(), endIterator, [&](detail::FireChainData const& n)
								{
									return (n.mStartPosition == sourcePosition && n.mEndPosition == targetPosition)
										|| (n.mStartPosition == targetPosition && n.mEndPosition == sourcePosition);
								});

							if (iterator == endIterator)
							{
								if (fireReceiverComponent->getId() == FireComponent::COMPONENT_ID)
								{
									// We have a fire component push it to the list of to be handled items
									fireComponentsList.push(static_cast<FireComponent*>(fireReceiverComponent));
								}

								// Add information for the line
								const float distance = glm::distance(targetPosition, sourcePosition);
								const qsf::Color4& lineColor = (distance > fireComponent->getHardRadius()) ? qsf::Color4::YELLOW : qsf::Color4::RED;
								fireChainDataList.emplace_back(sourcePosition, targetPosition, lineColor);
							}
						}
					}
				}
			}

			// Add debug draw requests for the fire chain lines
			for (const detail::FireChainData& fireChainData : fireChainDataList)
			{
				mDebugDrawProxy.addRequest(qsf::SegmentDebugDrawRequest(qsf::Segment::fromTwoPoints(fireChainData.mStartPosition, fireChainData.mEndPosition + glm::vec3(0.0f, 0.2f, 0.0f)), fireChainData.mLineColor));
			}

			// Add debug draw requests for the fire receiver position circles
			for (const glm::vec3& positionCirclePosition : fireComponentPostionCircles)
			{
				mDebugDrawProxy.addRequest(qsf::CircleDebugDrawRequest(positionCirclePosition, qsf::CoordinateSystem::getUp(), 0.125f, qsf::Color4::ORANGE, true));
			}
		}

		bool FireSimulationTool::getFireReceiverInSphere(const glm::vec3& center, float radius, const qsf::ComponentCollection::ComponentList<FireReceiverComponent>& allFireReceiverList, std::vector<FireReceiverComponent*>& outFireReceivers)
		{
			if (radius > 0)
			{
				for (FireReceiverComponent* fireReceiverComponent : allFireReceiverList)
				{
					// Check whether the entity is near enough
					const qsf::TransformComponent* transformComponent = fireReceiverComponent->getTransformComponent();
					if (nullptr != transformComponent)
					{
						const glm::vec3& position = transformComponent->getPosition();
						if (glm::distance(position, center) <= radius)
						{
							// Add this component
							outFireReceivers.push_back(fireReceiverComponent);
						}
					}
				}
			}

			return !outFireReceivers.empty();
		}

		void FireSimulationTool::updateFireComponentListModel()
		{
			QListView* qListView = mUiFireSimulationTool->fireComponentListView;
			const qsf::editor::EditModeManager& editModeManager = QSF_EDITOR_EDITMODE_MANAGER;
			if (editModeManager.getSelectedEditMode() == editModeManager.get<FireComponentEditMode>())
			{
				// Update only selection state
				for (uint64 entityId : mEntitySelectionManager.getSelectedIdSet())
				{
					// Check if the selected entity is known by the model
					QModelIndexList resultSet = mFireComponentListModel->match(mFireComponentListModel->index(0, 0), FireComponentListModel::FireEntityIdRole, entityId);
					if (!resultSet.empty())
					{
						// We found it
						// -> Default match returns on the first hit  -> we have only one item in the result set
						qListView->setCurrentIndex(resultSet[0]);
						return;
					}
				}

				// The selected entities are not known by the model -> clear the selection
				qListView->selectionModel()->clear();
			}
			else
			{
				// Unset inspected object in property widget
				qListView->selectionModel()->clear();
				qListView->setCurrentIndex(QModelIndex());

				// Get all fire components of the selected entities
				std::vector<FireComponent*> fireComponents;
				getFireComponentsFromSelectedEntities(QSF_MAINMAP, fireComponents);

				// Update the model with the new list
				mFireComponentListModel->updateFireComponentList(fireComponents);

				if (!fireComponents.empty())
				{
					// Preselect the first item in list
					qListView->setCurrentIndex(mFireComponentListModel->index(0, 0));
				}
			}
		}

		void FireSimulationTool::enableDisableButtonsForEditMode(bool enabled)
		{
			mUiFireSimulationTool->pushButtonHideFireChainsForAllItems->setEnabled(enabled);
			mUiFireSimulationTool->pushButtonHideFireRadiansForAllItems->setEnabled(enabled);
			mUiFireSimulationTool->pushButtonShowFireChainsForSelectedItems->setEnabled(enabled);
			mUiFireSimulationTool->pushButtonShowFireRadiansForSelectedItems->setEnabled(enabled);
			mUiFireSimulationTool->pushButtonShowHideFireRadiansForAllItems->setEnabled(enabled);

			// Buttons which are enabled when edit mode is active
			mUiFireSimulationTool->pushButtonCreateEM5Fire->setEnabled(!enabled);
			mUiFireSimulationTool->pushButtonDeleteEM5Fire->setEnabled(!enabled);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
