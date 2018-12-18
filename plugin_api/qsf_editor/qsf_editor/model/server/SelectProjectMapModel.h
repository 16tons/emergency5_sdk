// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetSystemTypes.h>

#include <QtCore/qabstractitemmodel.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			struct EditorMapInfo;
			struct EditorMapProjectInfo;
			struct EditorMapProjectList;
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
		*    Select project map Qt model
		*/
		class SelectProjectMapModel : public QAbstractTableModel
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
			enum PROJECT_MAP_ROLES
			{
				ProjectMapIdRole = Qt::UserRole + 1
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
			explicit SelectProjectMapModel(QObject* parent);

			/**
			*  @brief
			*    Destructor deletes the allocated memory
			*/
			virtual ~SelectProjectMapModel();

			/**
			*   @brief
			*     Refreshes the model data with the new editor project map list
			*
			*  @param[in] mapProjectList
			*    The new project map list
			*/
			void updateMapList(const base::EditorMapProjectList& mapProjectList);


		//[-------------------------------------------------------]
		//[ Public virtual QAbstractItemModel methods             ]
		//[-------------------------------------------------------]
		public:
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
			virtual QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;
			virtual int rowCount(const QModelIndex& parent = QModelIndex()) const override;
			virtual int columnCount(const QModelIndex& parent = QModelIndex()) const override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			struct ProjectMapInfo
			{
				GlobalAssetId mGlobalAssetId;
				ProjectId     mProjectId;
				QString		  mProjectName;
				QString		  mMapName;

				ProjectMapInfo();
				ProjectMapInfo(const base::EditorMapProjectInfo& projectInfo, const base::EditorMapInfo& mapInfo);
			};


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void addOrUpdateProjectMap(const base::EditorMapProjectInfo& projectInfo, const base::EditorMapInfo& mapInfo);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			QVector<ProjectMapInfo> mProjectMaps;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
