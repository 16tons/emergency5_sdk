// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/model/FireComponentListModel.h"

#include <em5/fire/component/FireComponent.h>

#include <qsf_editor_base/operation/component/CreateComponentOperation.h>
#include <qsf_editor_base/operation/component/DestroyComponentOperation.h>
#include <qsf_editor_base/operation/entity/CreateEntityOperation.h>
#include <qsf_editor_base/operation/entity/DestroyEntityOperation.h>

#include <qsf/QsfHelper.h>
#include <qsf/map/Entity.h>
#include <qsf/map/EntityHelper.h>
#include <qsf/reflection/CampHelper.h>
#include <qsf/reflection/ClassSystem.h>

#include <QtCore/QCoreApplication>


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


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		FireComponentListModel::FireComponentListModel(QObject* parent)
			: QAbstractListModel(parent)
		{
		}

		FireComponentListModel::~FireComponentListModel()
		{
		}

		void FireComponentListModel::updateFireComponentList(const std::vector<em5::FireComponent*>& fireComponentList)
		{
			// Inform all views that we know change the model in a "complex" way (which cannot be represented by insert/remove row(s) actions)
			beginResetModel();

			mFireComponents.clear();
			for (FireComponent* fireComponent : fireComponentList)
			{
				mFireComponents.emplace_back(fireComponent->getEntityId(), fireComponent);
			}

			// Now we are done
			endResetModel();
		}

		void FireComponentListModel::onUndoOperationExecuted(const qsf::editor::base::Operation& operation)
		{
			const uint32 operationId = operation.getId();

			if (qsf::editor::base::CreateComponentOperation::OPERATION_ID == operationId)
			{
				const qsf::editor::base::CreateComponentOperation& createComponentOperation = static_cast<const qsf::editor::base::CreateComponentOperation&>(operation);
				if (createComponentOperation.getComponentId() == FireComponent::COMPONENT_ID)
				{
					removeFireComponentByEntity(createComponentOperation.getEntityId());
				}
			}
			else if (qsf::editor::base::DestroyComponentOperation::OPERATION_ID == operationId)
			{
				const qsf::editor::base::DestroyComponentOperation& destroyComponentOperation = static_cast<const qsf::editor::base::DestroyComponentOperation&>(operation);
				if (destroyComponentOperation.getComponentId() == FireComponent::COMPONENT_ID)
				{
					addFireComponentByEntity(destroyComponentOperation.getEntity(), destroyComponentOperation.getComponent());
				}
			}
			else if (qsf::editor::base::DestroyEntityOperation::OPERATION_ID == operationId)
			{
				const qsf::editor::base::DestroyEntityOperation& destroyEntityOperation = static_cast<const qsf::editor::base::DestroyEntityOperation&>(operation);
				FireComponent* fireComponent = destroyEntityOperation.getEntity()->getComponent<FireComponent>();
				if (nullptr != fireComponent)
				{
					addFireComponentByEntity(destroyEntityOperation.getEntity(), fireComponent);
				}
			}
			else if (qsf::editor::base::CreateEntityOperation::OPERATION_ID == operationId)
			{
				// We have no way to determine if the entity had a fire component because the entity and their components are already gone
				const qsf::editor::base::CreateEntityOperation& createEntityOperation = static_cast<const qsf::editor::base::CreateEntityOperation&>(operation);
				removeFireComponentByEntity(createEntityOperation.getEntityId());
			}
		}

		void FireComponentListModel::onRedoOperationExecuted(const qsf::editor::base::Operation& operation)
		{
			const uint32 operationId = operation.getId();

			if (qsf::editor::base::CreateComponentOperation::OPERATION_ID == operationId)
			{
				const qsf::editor::base::CreateComponentOperation& createComponentOperation = static_cast<const qsf::editor::base::CreateComponentOperation&>(operation);
				if (createComponentOperation.getComponentId() == FireComponent::COMPONENT_ID)
				{
					addFireComponentByEntity(createComponentOperation.getEntity(), createComponentOperation.getComponent());
				}
			}
			else if (qsf::editor::base::DestroyComponentOperation::OPERATION_ID == operationId)
			{
				const qsf::editor::base::DestroyComponentOperation& destroyComponentOperation = static_cast<const qsf::editor::base::DestroyComponentOperation&>(operation);
				if (destroyComponentOperation.getComponentId() == FireComponent::COMPONENT_ID)
				{
					removeFireComponentByEntity(destroyComponentOperation.getEntityId());
				}
			}
			else if (qsf::editor::base::DestroyEntityOperation::OPERATION_ID == operationId)
			{
				// We have no way to determine if the entity had a fire component because the entity and their components are already gone
				const qsf::editor::base::DestroyEntityOperation& destroyEntityOperation = static_cast<const qsf::editor::base::DestroyEntityOperation&>(operation);
				removeFireComponentByEntity(destroyEntityOperation.getEntityId());
			}
			else if (qsf::editor::base::CreateEntityOperation::OPERATION_ID == operationId)
			{
				const qsf::editor::base::CreateEntityOperation& createEntityOperation = static_cast<const qsf::editor::base::CreateEntityOperation&>(operation);
				FireComponent* fireComponent = createEntityOperation.getEntity()->getComponent<FireComponent>();
				if (nullptr != fireComponent)
				{
					addFireComponentByEntity(createEntityOperation.getEntity(), fireComponent);
				}
			}
		}


		//[-------------------------------------------------------]
		//[ Protected virtual QAbstractListModel methods          ]
		//[-------------------------------------------------------]
		int FireComponentListModel::rowCount(const QModelIndex&) const
		{
			// We have only a simply list -> parent doesn't matter
			// So we can return here simply the number of items in the intern list as row count
			return static_cast<int>(mFireComponents.size());
		}

		QVariant FireComponentListModel::data(const QModelIndex& index, int role) const
		{
			if (!index.isValid() || index.row() >= static_cast<int>(mFireComponents.size()) || index.column() != 0)
				return QVariant();

			const FireComponentData& fireComponentData = mFireComponents[index.row()];

			if (role == Qt::DisplayRole)
			{
				// Display component and child entity name
				static const camp::Class& campClass = camp::classByType<FireComponent>();
				QString className = QCoreApplication::translate("", qsf::CampHelper::getCampTagValue<std::string>(campClass, "Name", campClass.name()).c_str());

				return className + " - " + QString::fromUtf8(qsf::EntityHelper(fireComponentData.mFireComponent->getEntity()).getName().c_str());
			}
			else if (role == Qt::UserRole)
			{
				return QVariant::fromValue<em5::FireComponent*>(fireComponentData.mFireComponent);
			}
			else if (role == FireEntityIdRole)
			{
				return fireComponentData.mEntityId;
			}

			return QVariant();
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		void FireComponentListModel::addFireComponentByEntity(const qsf::Entity* entity, qsf::Component* component)
		{
			QSF_ASSERT(component != nullptr, "The component pointer should be valid", return)
			int newRowIdx = static_cast<int>(mFireComponents.size());

			beginInsertRows(QModelIndex(), newRowIdx, newRowIdx);
			mFireComponents.emplace_back(component->getEntityId(), static_cast<FireComponent*>(component));
			endInsertRows();
		}

		void FireComponentListModel::removeFireComponentByEntity(const uint64 entityId)
		{
			auto iterator = mFireComponents.begin();
			auto endIterator = mFireComponents.end();
			int rowIdx = 0;
			while (iterator != endIterator)
			{
				const FireComponentData& componentData = *iterator;
				if (componentData.mEntityId == entityId)
				{
					beginRemoveRows(QModelIndex(), rowIdx, rowIdx);
					mFireComponents.erase(iterator);
					endRemoveRows();
					return;
				}
				++iterator;
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
