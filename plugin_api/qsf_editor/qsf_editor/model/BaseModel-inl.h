// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public qsf::editor::BaseModel::IndexData methods      ]
		//[-------------------------------------------------------]
		inline BaseModel::IndexData::IndexData() :
			indexType(INDEXTYPE_UNDEFINED),
			objectId(getUninitialized<uint64>()),
			object(nullptr)
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline BaseModel::~BaseModel()
		{
			if (nullptr != mRootTreeItem)
			{
				delete mRootTreeItem;
				mRootTreeItem = nullptr;
			}
		}

		inline BaseModel::IndexData* BaseModel::getIndexDataByQModelIndex(const QModelIndex& qModelIndex) const
		{
			if (qModelIndex.isValid())
			{
				TreeItem<IndexData>* treeItem = static_cast<TreeItem<IndexData>*>(qModelIndex.internalPointer());
				return (nullptr != treeItem) ? treeItem->data(0) : nullptr;
			}
			return nullptr;
		}

		inline TreeItem<BaseModel::IndexData>* BaseModel::getTreeItemByQModelIndex(const QModelIndex& qModelIndex) const
		{
			return qModelIndex.isValid() ? static_cast<TreeItem<IndexData>*>(qModelIndex.internalPointer()) : nullptr;
		}

		inline BaseModel::IndexData* BaseModel::getIndexDataByObjectPropertyIndex(const Object& object, uint32 propertyIndex) const
		{
			TreeItem<IndexData>* treeItem = findObjectPropertyIndex(object, propertyIndex, *mRootTreeItem);
			return (nullptr != treeItem) ? treeItem->data() : nullptr;
		}

		inline TreeItem<BaseModel::IndexData>* BaseModel::getTreeItemByObjectPropertyIndex(const Object& object, uint32 propertyIndex) const
		{
			return findObjectPropertyIndex(object, propertyIndex, *mRootTreeItem);
		}

		inline BaseModel::IndexData* BaseModel::getIndexDataByEntityId(uint64 entityId) const
		{
			TreeItem<IndexData>* treeItem = findEntityId(entityId, *mRootTreeItem);
			return (nullptr != treeItem) ? treeItem->data() : nullptr;
		}

		inline TreeItem<BaseModel::IndexData>* BaseModel::getTreeItemByEntityId(uint64 entityId) const
		{
			return findEntityId(entityId, *mRootTreeItem);
		}

		inline BaseModel::IndexData* BaseModel::getIndexDataByLayerId(uint64 layerId) const
		{
			TreeItem<IndexData>* treeItem = findLayerId(layerId, *mRootTreeItem);
			return (nullptr != treeItem) ? treeItem->data() : nullptr;
		}

		inline TreeItem<BaseModel::IndexData>* BaseModel::getTreeItemByLayerId(uint64 layerId) const
		{
			return findLayerId(layerId, *mRootTreeItem);
		}

		inline BaseModel::IndexData* BaseModel::getIndexDataByEntityComponentId(uint64 entityId, uint32 componentId) const
		{
			TreeItem<IndexData>* treeItem = getTreeItemByEntityComponentId(entityId, componentId);
			return (nullptr != treeItem) ? treeItem->data() : nullptr;
		}

		inline TreeItem<BaseModel::IndexData>* BaseModel::getTreeItemByEntityComponentId(uint64 entityId, uint32 componentId) const
		{
			TreeItem<BaseModel::IndexData>* parentTreeItem = getTreeItemByEntityId(entityId);
			if (nullptr == parentTreeItem)
			{
				parentTreeItem = mRootTreeItem;
			}
			return (nullptr != parentTreeItem) ? findComponentId(componentId, *parentTreeItem) : nullptr;
		}

		inline Object* BaseModel::getObjectFromBaseModelIndex(const QModelIndex& qModelIndex) const
		{
			const IndexData* indexData = getIndexDataByQModelIndex(qModelIndex);
			return (nullptr != indexData) ? indexData->object : nullptr;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
