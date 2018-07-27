// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/model/BaseModel.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	namespace editor
	{
		class EntityPreviewManager;
		namespace base
		{
			class Operation;
			class LayerOperation;
			class EntityOperation;
			class SetComponentPropertyOperation;
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
		*/
		class ObjectBrowserModel : public BaseModel
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
			ObjectBrowserModel(QObject* parent, Map& map);
			virtual ~ObjectBrowserModel();

			/**
			*  @brief
			*    Refresh the internal state of the model
			*
			*  @note
			*    - All model data are re-created from the data of the map; This causes an reset in the model ("begin"/"endReset" model signals are emitted)
			*/
			void refresh();

			void onUndoOperationExecuted(const base::Operation& operation);
			void onRedoOperationExecuted(const base::Operation& operation);


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			inline virtual int columnCount(const QModelIndex& parent) const override;
			inline virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
			virtual QVariant data(const QModelIndex& index, int role) const override;
			virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
			virtual QStringList mimeTypes() const override;
			virtual QMimeData* mimeData(const QModelIndexList& indexes) const override;
			virtual bool canDropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) const override;
			virtual bool dropMimeData(const QMimeData* data, Qt::DropAction action, int row, int column, const QModelIndex& parent) override;
			virtual bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
			virtual bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
			virtual bool moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild) override;
			virtual Qt::ItemFlags flags(const QModelIndex& index) const override;
			virtual Qt::DropActions supportedDropActions() const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void setupModelData(const Layer& parentLayer, TreeItem<IndexData>& parentItem);
			// Layer
			void createLayer(const base::LayerOperation& layerOperation, uint32 parentLayerId);
			void destroyLayer(const base::LayerOperation& layerOperation);
			void refreshLayer(const base::LayerOperation& layerOperation);
			void hideUnhideLayer(const base::LayerOperation& layerOperation);
			// Entity
			void createEntity(const base::EntityOperation& entityOperation, unsigned int layerId);
			void destroyEntity(const base::EntityOperation& entityOperation);
			void refreshEntity(const base::EntityOperation& entityOperation);
			// Entity component
			void setComponentProperty(const base::SetComponentPropertyOperation& setComponentPropertyOperation);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			EntityPreviewManager& mEntityPreviewManager;
			Map&				  mMap;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/model/object/ObjectBrowserModel-inl.h"
