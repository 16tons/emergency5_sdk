// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"
#include "qsf_editor/model/TreeItem.h"

#include <QtCore/qabstractitemmodel.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Property;
}
namespace qsf
{
	class Layer;
	class Entity;
	class Object;
	class Component;
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
		*    Base class for all editor relevant models
		*
		*  @remarks
		*    This is class saves all information needed for reconstructing every object/parent relations.
		*    So we can get access to a Qt model index via entity ID or get the component of a property.
		*/
		class QSF_EDITOR_API_EXPORT BaseModel : public QAbstractItemModel
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
			enum IndexType
			{
				INDEXTYPE_UNDEFINED,
				INDEXTYPE_ENTITY,
				INDEXTYPE_COMPONENT,
				// TODO(mk) INDEXTYPE_ARRAY is not supported, we need a better technical solution for referencing on camp::Property.
				//INDEXTYPE_ARRAY,		///< An array is technical a property, but it is also a array, here we decide between them to make it easier in handling
				INDEXTYPE_PROPERTY,
				INDEXTYPE_LAYER,
				INDEXTYPE_EXTEND		///< Indicates the next free index type which can be used by derived models
			};

			struct IndexData
			{
				// Item
				IndexType	indexType;	///< Information about the type behind the qModelIndex
				uint64		objectId;	///< Object ID, depends on the index type (layer, entity etc.)
				Object*		object;		///< Points to the data, check indexType and cast without checking, point to a component in case the item type is property


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				inline IndexData();
				Layer& getLayerSafe() const;
				Entity& getEntitySafe() const;
				Component& getComponentSafe() const;


			};


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] parent
			*    Optional parent, can be a null pointer, in case there's a parent it will take over the (memory) control of this model
			*/
			explicit BaseModel(QObject* parent);

			/**
			*  @brief
			*    Destructor deletes the allocated memory
			*/
			inline virtual ~BaseModel();

			// TODO(mk) think about making this function static (it's only a cast, and we need it often)
			inline IndexData* getIndexDataByQModelIndex(const QModelIndex& qModelIndex) const;
			inline TreeItem<IndexData>* getTreeItemByQModelIndex(const QModelIndex& qModelIndex) const;

			inline IndexData* getIndexDataByObjectPropertyIndex(const Object& object, uint32 propertyIndex) const;
			inline TreeItem<IndexData>* getTreeItemByObjectPropertyIndex(const Object& object, uint32 propertyIndex) const;

			inline IndexData* getIndexDataByEntityId(uint64 entityId) const;
			inline TreeItem<IndexData>* getTreeItemByEntityId(uint64 entityId) const;

			inline IndexData* getIndexDataByLayerId(uint64 layerId) const;
			inline TreeItem<IndexData>* getTreeItemByLayerId(uint64 layerId) const;

			inline IndexData* getIndexDataByEntityComponentId(uint64 entityId, uint32 componentId) const;
			inline TreeItem<IndexData>* getTreeItemByEntityComponentId(uint64 entityId, uint32 componentId) const;

			/**
			*  @brief
			*    Return the data from the given Qt model index
			*
			*  @param[in] qModelIndex
			*    Qt model index which holds data
			*
			*  @return
			*    Pointer to the data from the given Qt model index, can be a null pointer
			*
			*  @remarks
			*    Simple models don't use the index data architecture, so they return the data from the given index.
			*    Otherwise we return the data from our own structure. eg ("return getIndexDataByQModelIndex(index)->object;").
			*    This method is used from outside, in cases where other classes can't know how the Qt model index is used to save data.
			*
			*  @note
			*    - In classes which use the base model index data architecture, reimplement this class // TODO(co) Review this comment, I don't get it
			*/
			inline Object* getObjectFromBaseModelIndex(const QModelIndex& qModelIndex) const;

			/**
			*  @brief
			*    Return the CAMP property from the given Qt model index
			*
			*  @param[in] qModelIndex
			*    Qt model index which holds data to a CAMP property
			*
			*  @return
			*    Pointer to the CAMP property from the given index, if no property found, return null pointer
			*
			*  @remarks
			*    Simple models don't use the index data architecture, so they return the data from the given index.
			*    Otherwise we return the data from our own structure. eg ("return getIndexDataByQModelIndex(index)->object;").
			*    This method is used from outside, in cases where other classes can't know how the Qt model index is used to save data.
			*
			*  @note
			*    - In classes which use the base model index data architecture, reimplement this class // TODO(co) Review this comment, I don't get it
			*/
			// TODO(sw) This method is never called anymore now that the functionality is moved to use an custom item data role
			const camp::Property* getPropertyFromObjectModelIndex(const QModelIndex& qModelIndex) const;

			/**
			*  @brief
			*    Return the Qt model index for the given tree item
			*
			*  @param[in] treeItem
			*    The tree item for which the Qt model index should be returned
			*
			*  @return
			*    Qt model index which holds data about the tree item, or an invalid Qt model index when either the parameter is a nullptr or the given tree item has no parent
			*/
			QModelIndex getIndexForTreeItem(TreeItem<IndexData>* treeItem) const;


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			virtual QModelIndex index(int row, int column, const QModelIndex& parent) const override;
			virtual QModelIndex parent(const QModelIndex& child) const override;
			virtual int rowCount(const QModelIndex& parent) const override;
			virtual int columnCount(const QModelIndex& parent) const override;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			TreeItem<IndexData>& getRootTreeItem() const;
			void removeTreeItem(TreeItem<IndexData>& parentTreeItem, int row);
			TreeItem<IndexData>* insertTreeItem(TreeItem<IndexData>& parentTreeItem, int row);
			void moveTreeItem(TreeItem<IndexData>& sourceParentItem, int sourceRow, TreeItem<IndexData>& destinationParentItem, int destinationRow);
			bool isInsertingNewTreeItemPossible(TreeItem<IndexData>& parentTreeItem);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Helper functions for recursive search
			TreeItem<IndexData>* findObjectPropertyIndex(const Object& object, uint32 propertyIndex, TreeItem<IndexData>& treeItem) const;
			TreeItem<IndexData>* findEntityId(uint64 entityId, TreeItem<IndexData>& treeItem) const;
			TreeItem<IndexData>* findLayerId(uint64 layerId, TreeItem<IndexData>& treeItem) const;
			TreeItem<IndexData>* findComponentId(uint64 componentId, TreeItem<IndexData>& treeItem) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		protected:
			TreeItem<IndexData>* mRootTreeItem;	///< Root tree item, always valid, we're responsible for destroying the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/model/BaseModel-inl.h"
