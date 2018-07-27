// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor/model/BaseModel.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireSystem;
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
		*    Fire materials Qt model
		*
		*  @todo
		*    - TODO(co) Implement me
		*/
		class FireMaterialsModel : public qsf::editor::BaseModel
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
				INDEXTYPE_FIREMATERIAL = qsf::editor::BaseModel::IndexType::INDEXTYPE_EXTEND		///< TODO(co) Editor: Revision: 5306 "qsf editor: add fire material type in base model": There's no EMERGENCY 5 fire material in QSF. That's an EMERGENCY 5 special thingy. Please try to keep this EMERGENCY 5 editor local.
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
			FireMaterialsModel(QObject* parent, FireSystem& fireSystem);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~FireMaterialsModel();

			/**
			*  @brief
			*    Enable/disable editing of all items in the model
			*/
			void enableEditing(bool enabled);

			/**
			*  @brief
			*    Let the model refresh the hold data from the data source
			*/
			void refresh();


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			virtual QModelIndex parent(const QModelIndex& child) const override;
			virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const override;
			virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
			virtual bool setData(const QModelIndex& index, const QVariant& value, int role) override;
			virtual Qt::ItemFlags flags(const QModelIndex& index) const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void setupModelData(const FireSystem& fireSystem, qsf::editor::TreeItem<IndexData>* parentItem);
			void onFireMaterialsChanged();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			const FireSystem&	mFireSystem;
			bool				mEditingEnabled;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
