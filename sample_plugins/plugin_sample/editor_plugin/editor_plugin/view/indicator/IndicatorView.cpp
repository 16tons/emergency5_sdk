// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "editor_plugin/PrecompiledHeader.h"
#include "editor_plugin/view/indicator/IndicatorView.h"
#include "ui_IndicatorView.h" // Automatically created by Qt's uic (output directory is "tmp\qt\uic\qsf_editor" within the hand configured Visual Studio files, another directory when using CMake)

#include <game_plugin/component/IndicatorComponent.h>

#include <qsf_editor/operation/utility/RebuildGuiOperation.h>
#include <qsf_editor/application/manager/CameraManager.h>
#include <qsf_editor/EditorHelper.h>

#include <qsf_editor_base/operation/entity/CreateEntityOperation.h>
#include <qsf_editor_base/operation/entity/DestroyEntityOperation.h>
#include <qsf_editor_base/operation/component/CreateComponentOperation.h>
#include <qsf_editor_base/operation/component/DestroyComponentOperation.h>
#include <qsf_editor_base/operation/component/SetComponentPropertyOperation.h>

#include <qsf/map/Map.h>
#include <qsf/map/Entity.h>
#include <qsf/selection/EntitySelectionManager.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		const uint32 IndicatorView::PLUGINABLE_ID = qsf::StringHash("qsf::editor::IndicatorView");


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		IndicatorView::IndicatorView(qsf::editor::ViewManager* viewManager, QWidget* qWidgetParent) :
			View(viewManager, qWidgetParent),
			mUiIndicatorView(nullptr)
		{
			// Add the created Qt dock widget to the given Qt main window and tabify it for better usability
			addViewAndTabify(reinterpret_cast<QMainWindow&>(*qWidgetParent), Qt::RightDockWidgetArea);
		}

		IndicatorView::~IndicatorView()
		{
			// Destroy the UI view instance
			if (nullptr != mUiIndicatorView)
			{
				delete mUiIndicatorView;
			}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::View methods           ]
		//[-------------------------------------------------------]
		void IndicatorView::retranslateUi()
		{
			// Retranslate the content of the UI, this automatically clears the previous content
			mUiIndicatorView->retranslateUi(this);

			QComboBox* qComboBoxViewMode = mUiIndicatorView->comboBoxType;
			qComboBoxViewMode->clear();
			qComboBoxViewMode->addItem(tr("ID_INDICATORVIEW_ALL"));
			qComboBoxViewMode->addItem(tr("ID_INDICATORVIEW_WHITE"));
			qComboBoxViewMode->addItem(tr("ID_INDICATORVIEW_RED"));
			qComboBoxViewMode->addItem(tr("ID_INDICATORVIEW_YELLOW"));
			qComboBoxViewMode->addItem(tr("ID_INDICATORVIEW_GREEN"));
			qComboBoxViewMode->addItem(tr("ID_INDICATORVIEW_BLUE"));
			qComboBoxViewMode->setCurrentIndex(0);
		}

		void IndicatorView::changeVisibility(bool visible)
		{
			// Lazy evaluation: If the view is shown the first time, create its content
			if (visible && nullptr == mUiIndicatorView)
			{
				// Setup the view content
				QWidget* contentWidget = new QWidget(this);
				{
					// Load content to widget
					mUiIndicatorView = new Ui::IndicatorView();
					mUiIndicatorView->setupUi(contentWidget);
				}

				// Set content to view
				setWidget(contentWidget);

				// Connect Qt signals/slots
				connect(mUiIndicatorView->pushButtonSelect, SIGNAL(clicked(bool)), this, SLOT(onPushSelectButton(bool)));
				connect(mUiIndicatorView->comboBoxType, SIGNAL(currentIndexChanged(int)), this, SLOT(onCurrentIndexChanged(int)));
			}
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void IndicatorView::rebuildGui()
		{
			// Count all entities from the map
			int count = 0;
			for (const qsf::Entity* entity : QSF_MAINMAP.getEntities())
			{
				if (nullptr != entity->getComponent<user::IndicatorComponent>())
				{
					++count;
				}
			}
			// Update the number in spinbox
			mUiIndicatorView->spinBoxCounter->setValue(count);
		}

		void IndicatorView::componentCreated()
		{
			// Increase the counter in spinbox
			QSpinBox* spinBox = mUiIndicatorView->spinBoxCounter;
			int oldValue = spinBox->value();
			spinBox->setValue(++oldValue);
		}

		void IndicatorView::componentDestroyed()
		{
			// Decrease the counter in spinbox
			QSpinBox* spinBox = mUiIndicatorView->spinBoxCounter;
			int oldValue = spinBox->value();
			spinBox->setValue(--oldValue);
		}

		void IndicatorView::updateComponentData(uint64 entityId)
		{
			// TODO(user) fill with live
		}


		//[-------------------------------------------------------]
		//[ Protected virtual QWidget methods                     ]
		//[-------------------------------------------------------]
		void IndicatorView::showEvent(QShowEvent* qShowEvent)
		{
			// Call the base implementation
			View::showEvent(qShowEvent);

			// Perform a GUI rebuild to ensure the GUI content is up-to-date
			rebuildGui();

			// Connect Qt signals/slots
			connect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::undoOperationExecuted, this, &IndicatorView::onUndoOperationExecuted);
			connect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::redoOperationExecuted, this, &IndicatorView::onRedoOperationExecuted);
		}

		void IndicatorView::hideEvent(QHideEvent* qHideEvent)
		{
			// Call the base implementation
			View::hideEvent(qHideEvent);

			// Disconnect Qt signals/slots
			disconnect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::undoOperationExecuted, this, &IndicatorView::onUndoOperationExecuted);
			disconnect(&QSF_EDITOR_OPERATION, &qsf::editor::OperationManager::redoOperationExecuted, this, &IndicatorView::onRedoOperationExecuted);
		}


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		void IndicatorView::onPushSelectButton(const bool pressed)
		{
			// Cycle through all entities of the map
			for (const qsf::Entity* entity : QSF_MAINMAP.getEntities())
			{
				// Look for indicator component
				if (entity->getComponent<user::IndicatorComponent>() != nullptr)
				{
					// Get the unique entity id
					uint64 entityId = entity->getId();

					// Check if the current entity is selected
					qsf::EntitySelectionManager& entitySelectionManager = QSF_EDITOR_SELECTION_SYSTEM.getSafe<qsf::EntitySelectionManager>();
					if (!entitySelectionManager.isIdSelected(entityId))
					{
						// Select and fly camera to entity
						entitySelectionManager.setSelectionById(entityId);
						QSF_EDITOR_APPLICATION.getCameraManager().flyCameraToEntity(*entity);
						break;
					}
				}
			}
		}

		void IndicatorView::onCurrentIndexChanged(int index)
		{
			QString displayText(mUiIndicatorView->comboBoxType->itemText(index).toUtf8().constData());
			mUiIndicatorView->lineEditSelected->setText(displayText);
		}

		void IndicatorView::onUndoOperationExecuted(const qsf::editor::base::Operation& operation)
		{
			const uint32 operationId = operation.getId();
			if (qsf::editor::base::CreateComponentOperation::OPERATION_ID == operationId)
			{
				// Check whether or not the component is an Indicator component
				const qsf::editor::base::CreateComponentOperation* createComponentOperation = static_cast<qsf::editor::base::CreateComponentOperation*>(const_cast<qsf::editor::base::Operation*>(&operation));
				if (createComponentOperation->getComponentId() == user::IndicatorComponent::COMPONENT_ID)
				{
					// Inform the view that a component has been destroyed
					componentDestroyed();
				}
			}
			else if (qsf::editor::base::CreateEntityOperation::OPERATION_ID == operationId)
			{
				rebuildGui();
			}
			else if (qsf::editor::base::DestroyEntityOperation::OPERATION_ID == operationId || qsf::editor::base::DestroyComponentOperation::OPERATION_ID == operationId)
			{
				// Inform the view that a component has been created
				componentCreated();
			}
			else if (qsf::editor::base::SetComponentPropertyOperation::OPERATION_ID == operationId)
			{
				// Inform the view that the indicator component has been changed
				updateComponentData(static_cast<const qsf::editor::base::EntityOperation&>(operation).getEntityId());
			}
			else if (qsf::editor::RebuildGuiOperation::OPERATION_ID == operationId)
			{
				rebuildGui();
			}
		}

		void IndicatorView::onRedoOperationExecuted(const qsf::editor::base::Operation& operation)
		{
			const uint32 operationId = operation.getId();
			if (qsf::editor::base::CreateComponentOperation::OPERATION_ID == operationId)
			{
				// Check whether or not the component is an indicator component
				const qsf::editor::base::CreateComponentOperation* createComponentOperation = static_cast<qsf::editor::base::CreateComponentOperation*>(const_cast<qsf::editor::base::Operation*>(&operation));
				if (createComponentOperation->getComponentId() == user::IndicatorComponent::COMPONENT_ID)
				{
					// Inform the view browser that a component has been created
					componentCreated();
				}
			}
			else if (qsf::editor::base::CreateEntityOperation::OPERATION_ID == operationId)
			{
				// Check whether or not the component is an indicator component
				const qsf::editor::base::CreateEntityOperation* createEntityOperation = static_cast<qsf::editor::base::CreateEntityOperation*>(const_cast<qsf::editor::base::Operation*>(&operation));
				qsf::Entity* entity = QSF_MAINMAP.getEntityById(createEntityOperation->getEntityId());
				if (nullptr != entity && nullptr != entity->getComponent<user::IndicatorComponent>())
				{
					// Inform the view browser that a component has been created
					componentCreated();
				}
			}
			else if (qsf::editor::base::DestroyEntityOperation::OPERATION_ID == operationId || qsf::editor::base::DestroyComponentOperation::OPERATION_ID == operationId)
			{
				const uint32 componentId = static_cast<const qsf::editor::base::ComponentOperation&>(operation).getComponentId();
				if (componentId == IndicatorComponent::COMPONENT_ID)
				{
					// Inform the view that a component has been destroyed
					componentDestroyed();
				}
			}
			else if (qsf::editor::base::SetComponentPropertyOperation::OPERATION_ID == operationId)
			{
				// Inform the view that the indicator component has been changed
				updateComponentData(static_cast<const qsf::editor::base::EntityOperation&>(operation).getEntityId());
			}
			else if (qsf::editor::RebuildGuiOperation::OPERATION_ID == operationId)
			{
				rebuildGui();
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // user
