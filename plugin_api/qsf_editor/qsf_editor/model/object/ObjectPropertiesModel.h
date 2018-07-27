// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/model/BaseModel.h"

#include <qsf/prototype/helper/SlotHelper.h>

#include <boost/container/flat_set.hpp>
#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class BoostSignalComponent;
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
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
		*    Object browser Qt model
		*
		*  @remarks
		*    The layout for displaying properties in the model is this
		*    Metadata component
		*      Property 0
		*      Property n
		*    Transform component
		*      Property 0
		*      Property n
		*    Child entity 0
		*        Metadata component
		*          Property 0
		*          Property n
		*        Transform component
		*          Property 0
		*          Property n
		*        Child entity 0
		*          Metadata component
		*            Property 0
		*            Property n
		*          Transform component
		*            Property 0
		*            Property n
		*          Child entity 0
		*          Child entity 1
		*        Child entity 1
		*    Child entity 1
		*
		*    Properties can be an array, which are also expanded	// TODO(co) Not implemented, yet
		*    Array
		*        Property 0
		*        Property n
		*/
		class ObjectPropertiesModel : public BaseModel
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			ObjectPropertiesModel(QObject* parent, Entity& entity);
			inline virtual ~ObjectPropertiesModel();
			void setSelectedObject(Entity* newSelectedObject);


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			virtual QModelIndex parent(const QModelIndex& child) const override;
			virtual int rowCount(const QModelIndex& parent) const override;
			inline virtual int columnCount(const QModelIndex& parent) const override;
			virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
			virtual QVariant data(const QModelIndex& index, int role) const override;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
			virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;
			virtual QStringList mimeTypes() const override;
			virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
			virtual Qt::ItemFlags flags(const QModelIndex& index) const override;


		//[-------------------------------------------------------]
		//[ Private Qt slots (MOC)                                ]
		//[-------------------------------------------------------]
		private Q_SLOTS:
			// qsf::editor::OperationManager
			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Create the base model data structure
			*/
			void setupModelData(const Entity& parentEntity, TreeItem<IndexData>& parentItem, bool sendAddRowsSignal = false);
			void setupModelData(Component& parentComponent, TreeItem<IndexData>& parentItem, bool sendAddRowsSignal);

			/**
			*  @brief
			*    Get "qsf::BoostSignalComponent" instance
			*
			*  @return
			*    "qsf::BoostSignalComponent" instance, can be a null pointer, do not destroy the returned instance
			*/
			BoostSignalComponent* getBoostSignal() const;

			/**
			*  @brief
			*    Connect to Boost signals
			*/
			void connectToBoostSignals();

			/**
			*  @brief
			*    Disconnect to Boost signals
			*/
			void disconnectFromBoostSignals();

			/**
			*  @brief
			*    Called in case a component property value was changed
			*
			*  @param[in] component
			*    Component instance which had a property value change
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*
			*  @note
			*    - The unique class property identifier is generated by "qsf::StringHash" by using the class property name
			*      (e.g. qsf::StringHash("Position") generating the hash value 3091144222)
			*/
			void onComponentPropertyChange(const Component& component, uint32 propertyId);

			/**
			*  @brief
			*    Change the property of the given entity and component to the given value
			*
			*  @param[in] entity
			*    The entity with the component
			*  @param[in] componentId
			*    The ID of the component whitch hold the property
			*  @param[in] rowInParent
			*    The index of the property
			*  @param[in] newCampValue
			*    The new property value
			*
			*  @return
			*    The operation for changing the value of the given property, null pointer in case of no different to previous value, destroy the instance in case you no longer need it
			*
			*  @note
			*    - This function is for easly changing one property for entities outside of the model, use this for multi selection
			*/
			base::Operation* changePropertyFromEntity(Entity& entity, uint32 componentId, int rowInParent, const std::string& newCampValue);

			/**
			*  @brief
			*    Check if we have multiple selection for an entity
			*
			*  @return
			*    True if we have else false
			*/
			bool checkEntitySelection(const QModelIndex& index, const QVariant& value, int role);

			/**
			*  @brief
			*    Does an incremental update of the model data for the components of the given entity
			*
			*  @param[in] entity
			*    The entity for which to do the incremental update of the components data
			*  @param[in] parentItem
			*    The parent item which holds the model data for the components as childs
			*/
			void updateIncrementalComponentData(const qsf::Entity& entity, TreeItem<IndexData>& parentItem);

			/**
			*  @brief
			*    Does an incremental update of the model data for the properties of the given component
			*
			*  @param[in] component
			*    The component for which to do the incremental update of the property data
			*  @param[in] parentItem
			*    The parent item which holds the model data for the properties as childs
			*/
			void updateIncrementalComponentPropertyData(qsf::Component& component, TreeItem<IndexData>& parentItem);

			/**
			*  @brief
			*    Updates the model data about child entities (no incremental update support
			*
			*  @param[in] parentEntity
			*    The parent entity for which the data about child entities should be updated
			*  @param[in] parentItem
			*    The parent item which holds the model data for the child entities as childs
			*  @param[in] sendAddRowsSignal
			*    When "True" the methods beginInsertRows and endInsertRows are called when an new row is added to inform any view about the change
			*/
			void updateChildEntities(const qsf::Entity& parentEntity, TreeItem<IndexData>& parentItem, bool sendAddRowsSignal);

			TreeItem<IndexData>* addComponentData(Component& component, TreeItem<IndexData>& parentItem, bool sendAddRowsSignal);
			void removeComponentData(uint64 entityId, uint32 componentId);

			void removeEntityData(uint64 entityId);
			void addEntityData(qsf::Entity& entity);

			void getChildEntityTreeItems(TreeItem<IndexData>& parentItem, std::vector<TreeItem<IndexData>*>& resultList);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef boost::container::flat_set<uint64> EntityIdSet;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Map&													mMap;				///< Map instance to operate on, do not destroy the instance
			boost::container::flat_map<const void*, QModelIndex>	mDataToModelIndex;	///< "QModelIndex::internalPointer()" to "QModelIndex"-parent mapping
			uint64													mSelectedEntityId;	///< The ID of the selected entity (can contain child entities, but is definitly no child), "qsf::getUninitialized<uint64>()" if invalid
			EntityIdSet												mEntityIdSet;		///< List of all entity IDs this object properties model currently shows
			std::unordered_map<uint64, SlotHelper::SlotIdentifier>	mSlotLinkedChildren;

			boost::signals2::connection	mPropertyChangedConnection;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/model/object/ObjectPropertiesModel-inl.h"
