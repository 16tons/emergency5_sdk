// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/editmode/FireComponentEditMode.h"

#include <em5/fire/component/ComplexFireComponent.h>

#include <qsf_editor/editmode/transform/map/ObjectTransformEditMode.h>
#include <qsf_editor/renderer/RenderView.h>
#include <qsf_editor/selection/GizmoManager.h>
#include <qsf_editor/EditorHelper.h>

#include <qsf/component/base/TransformComponent.h>
#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/QsfHelper.h>

#include <QtGui/qevent.h>


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
		const uint32 FireComponentEditMode::PLUGINABLE_ID = qsf::StringHash("em5::editor::FireComponentEditMode");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		FireComponentEditMode::FireComponentEditMode(qsf::editor::EditModeManager* editModeManager) :
			EditMode(editModeManager)
		{
			// Nothing to do in here
		}

		FireComponentEditMode::~FireComponentEditMode()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		bool FireComponentEditMode::onStartup(qsf::editor::EditMode* previousEditMode)
		{
			// Call the base implementation
			if (!qsf::editor::EditMode::onStartup(previousEditMode))
				return false;	// Error!

			if (previousEditMode != QSF_EDITOR_EDITMODE_MANAGER.get<qsf::editor::ObjectTransformEditMode>())
			{
				qsf::editor::EntitySelectionManager& entitySelectionManager = QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>();

				// We allow only one selection for this mode...
				if (entitySelectionManager.getSelectionSize() != 1)
					return false;

				// ... and the selected entity must have a complex fire component...
				qsf::Entity* selectedEntity = QSF_MAINMAP.getEntityById(entitySelectionManager.getSelectedId());
				ComplexFireComponent* complexFireComponent = selectedEntity->getComponent<ComplexFireComponent>();
				if (nullptr == complexFireComponent)
					return false;

				// ... and at least one fire component must exists
				std::vector<FireComponent*> fireComponents;
				complexFireComponent->getAllFireComponents(fireComponents);
				if (fireComponents.empty())
				{
					return false;
				}
			}

			// Connect undo/redo operation Qt signals/slots
			connect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::undoOperationExecuted, this, &FireComponentEditMode::onUndoOperationExecuted);
			connect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::redoOperationExecuted, this, &FireComponentEditMode::onRedoOperationExecuted);

			updateGizmo();

			// Done
			return true;
		}

		void FireComponentEditMode::onShutdown(qsf::editor::EditMode* nextEditMode)
		{
			// Disconnect undo/redo operation Qt signals/slots
			disconnect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::undoOperationExecuted, this, &FireComponentEditMode::onUndoOperationExecuted);
			disconnect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::redoOperationExecuted, this, &FireComponentEditMode::onRedoOperationExecuted);

			// Call the base implementation
			qsf::editor::EditMode::onShutdown(nextEditMode);
		}

		void FireComponentEditMode::mousePressEvent(QMouseEvent& qMouseEvent)
		{
			// Left mouse button?
			if (Qt::LeftButton == qMouseEvent.button())
			{
				// We accept and process this Qt event (unwanted events might be propagated to the parent widget, we don't want that)
				qMouseEvent.accept();
				//qsf::editor::EntitySelectionManager& entitySelectionManager = QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>();	// TODO(co) Why is this commented?

				qsf::editor::GizmoManager& gizmoManager = QSF_EDITOR_SELECTION_SYSTEM.getGizmoManager();
				if (!gizmoManager.switchToTransformEditModeIfPossible(qMouseEvent, getRenderView().getRenderWindow(), QSF_EDITOR_EDITMODE_MANAGER.getSafe<qsf::editor::ObjectTransformEditMode>()))
				{
					// The gizmo wasn't clicked
					// -> clear selection?		// TODO(co) Is this a TODO?
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		void FireComponentEditMode::onUndoOperationExecuted(const qsf::editor::base::Operation& operation)
		{
			updateGizmo();
		}

		void FireComponentEditMode::onRedoOperationExecuted(const qsf::editor::base::Operation& operation)
		{
			updateGizmo();
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void FireComponentEditMode::updateGizmo()
		{
			// Place the gizmo over the currently selected entity
			qsf::Entity* entity = QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::editor::EntitySelectionManager>().getCurrentEntity();
			qsf::editor::GizmoManager& gizmoManager = QSF_EDITOR_SELECTION_SYSTEM.getGizmoManager();

			// We allow only entities with a fire component to have a gizmo (as a pet *g*)
			if (nullptr == entity || entity->getComponent<FireComponent>() == nullptr)
			{
				// Disable gizmo
				gizmoManager.setGizmoEnabled(false);
				return;
			}

			// In case we received an entity, get the transform component
			qsf::TransformComponent* transformComponent = entity->getComponent<qsf::TransformComponent>();

			// Update the gizmo manager
			gizmoManager.setFeatureSet(qsf::editor::GizmoManager::FeatureBitmask(qsf::editor::GizmoManager::MOVEMENT_FEATURE));
			if (nullptr != transformComponent)
			{
				// Enable gizmo and set correct transform
				gizmoManager.setGizmoEnabled(true);
				gizmoManager.setGizmoTransform(transformComponent->getTransform());
			}
			else
			{
				// Disable gizmo
				gizmoManager.setGizmoEnabled(false);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
