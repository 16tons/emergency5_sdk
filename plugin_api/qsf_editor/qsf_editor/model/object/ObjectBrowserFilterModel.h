// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/base/FilterConfiguration.h"

#include <QtCore/qsortfilterproxymodel.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Layer;
	namespace editor
	{
		class ObjectBrowserFilterManager;
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
		*    Object browser Qt filter model
		*
		*  @remarks
		*    In the filter model, we show layer at first, then all entities on this layer
		*    - Base layer
		*      - Sublayer 0
		*        - Entity 0
		*        - Entity n
		*      - Sublayer n
		*        - Entity 0
		*        - Entity n
		*    - Layer 0
		*    - Layer n
		*
		*  @note
		*    - This class filters entities from the user view and sort them alphabeticaly
		*    - See http://qt-project.org/doc/qt-4.8/qsortfilterproxymodel.html as reference
		*    - Root layer == QModelIndex()
		*/
		class ObjectBrowserFilterModel : public QSortFilterProxyModel
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
			ObjectBrowserFilterModel(QObject* parent, bool filterSelection, const FilterConfiguration& filterConfiguration);
			virtual ~ObjectBrowserFilterModel();
			void setFilterConfiguration(const FilterConfiguration& newFilterConfiguration);


		//[-------------------------------------------------------]
		//[ Protected virtual QSortFilterProxyModel methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;
			virtual bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Check if this entity is allowed to show from filter
			*
			*  @param[in] entity
			*    Checked entity
			*
			*  @return
			*    "true" if item can be shown, else "false"
			*/
			bool acceptEntity(const Entity& entity) const;

			/**
			*  @brief
			*    Check if this layer is allowed to show from filter
			*
			*  @param[in] layer
			*    Checked layer
			*
			*  @return
			*    Returns "true" if item can be shown, else "false"
			*
			*  @note
			*    - Calls itself recursively for all sublayer inside the layer
			*/
			bool acceptLayer(const Layer& layer) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool				mFilterSelection;		// If true, the model is filtering the selection in case of multiselection
			FilterConfiguration	mFilterConfiguration;
			ObjectBrowserFilterManager& mFilterManager;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
