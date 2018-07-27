// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/model/asset/AssetFilterModel.h"


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
		*    Prefab browser Qt filter model
		*/
		class PrefabBrowserFilterModel : public AssetFilterModel
		{


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
			explicit PrefabBrowserFilterModel(QObject* parent);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~PrefabBrowserFilterModel();

			void setForceFilteringByCategory(bool force);


		//[-------------------------------------------------------]
		//[ Protected virtual QSortFilterProxyModel methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
			virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mForceFilteringByCategory;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
