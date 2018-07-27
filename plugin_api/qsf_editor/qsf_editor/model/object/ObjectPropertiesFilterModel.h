// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <QtCore/qsortfilterproxymodel.h>


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
namespace camp
{
	class Property;
}
namespace qsf
{
	class Entity;
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
		*    Object property Qt filter model
		*
		*  @note
		*    - This class filters components and properties from the user view
		*    - See http://qt-project.org/doc/qt-4.8/qsortfilterproxymodel.html as reference
		*/
		class ObjectPropertiesFilterModel : public QSortFilterProxyModel
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
			inline explicit ObjectPropertiesFilterModel(QObject* parent);
			inline virtual ~ObjectPropertiesFilterModel();


		//[-------------------------------------------------------]
		//[ Protected virtual QSortFilterProxyModel methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Check if this entity is allowed to show from filter
			*
			*  @param[in] entity
			*    Reference to checked entity
			*
			*  @return
			*    "true" if item can be shown, else "false"
			*/
			bool acceptEntity(const Entity& entity) const;

			/**
			*  @brief
			*    Check if this component is allowed to show from filter
			*
			*  @param[in] component
			*    Reference to checked component
			*
			*  @return
			*    "true" if item can be shown, else "false"
			*/
			bool acceptComponent(const Component& component) const;

			/**
			*  @brief
			*    Check if this property is allowed to show from filter
			*
			*  @param[in] property
			*    Reference to checked property
			*
			*  @return
			*    "true" if item can be shown, else "false"
			*/
			bool acceptProperty(const camp::Property& property) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			mutable bool mSettingsAllowFilteringHiddenTag;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/model/object/ObjectPropertiesFilterModel-inl.h"
