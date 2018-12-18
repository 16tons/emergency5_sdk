// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/entity/EntitySelectionManager.h"

#include <QtCore/qmimedata.h>

#include <boost/property_tree/ptree_fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
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
		*    Component array property value Qt mime data
		*
		*  @note
		*    - Stores information about 1 component array property value
		*/
		class QSF_EDITOR_API_EXPORT ComponentArrayPropertyValueMimeData : public QMimeData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const QString MIME_TYPE;	///< Mime type "qsf/componentarraypropertyvalue"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor for creating componentarraypropertyvalue Qt mime data by using a given component
			*
			*  @param[in] campUserObject
			*    The camp campProperty object from which data should be copied
			*  @param[in] campUserObject
			*    The camp property which data should be copied
			*  @param[in] mimeType
			*    Direct reference to the concrete mime type
			*/
			ComponentArrayPropertyValueMimeData(const camp::UserObject& campUserObject, const camp::Property& campProperty, const QString& mimeType = MIME_TYPE);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~ComponentArrayPropertyValueMimeData();

			/**
			*  @brief
			*    Return the component property data as camp value stored in the mime data
			*
			*  @param[out] pTree
			*    The boost ptree which will hold the component property data
			*/
			void getComponentPropertyValeAsBoostPTree(boost::property_tree::ptree& pTree) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			const QString& mMimeType;	///< Direct reference to the concrete mime type


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/mimedata/ComponentArrayPropertyValueMimeData-inl.h"
