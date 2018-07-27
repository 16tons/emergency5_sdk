// Copyright (C) 2012-2017 Promotion Software GmbH


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
		*    Copy Component Qt mime data
		*
		*  @note
		*    - Stores information about 1 component and its data
		*/
		class QSF_EDITOR_API_EXPORT CopyComponentMimeData : public QMimeData
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
			static const QString MIME_TYPE;	///< Mime type "qsf/copycomponent"


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return the component type name stored in the mime data
			*
			*  @param[int] qmimeData
			*    The Qt mime data object from which the data should be retrieved
			*
			*  @return
			*    Component type name
			*/
			static std::string getComponentTypeName(const QMimeData& qmimeData);

			/**
			*  @brief
			*    Return the component type stored in the mime data
			*
			*  @param[int] qmimeData
			*    The Qt mime data object from which the data should be retrieved
			*
			*  @return
			*    Component type name
			*/
			static uint32 getComponentType(const QMimeData& qmimeData);

			/**
			*  @brief
			*    Return the component data as boost ptree stored in the mime data
			*
			*  @param[int] qmimeData
			*    The Qt mime data object from which the data should be retrieved
			*  @param[out] pTree
			*    The boost ptree which will hold the component data
			*/
			static void getComponentDataAsBoostPTree(const QMimeData& qmimeData, boost::property_tree::ptree& pTree);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor for creating "qsf/copycomponent" Qt mime data by using a given component
			*
			*  @param[in] component
			*    The component which should be copied
			*  @param[in] mimeType
			*    Direct reference to the concrete mime type
			*/
			CopyComponentMimeData(Component& component, const QString& mimeType = MIME_TYPE);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CopyComponentMimeData();

			/**
			*  @brief
			*    Return the component type name stored in the mime data
			*
			*  @return
			*    Component type name
			*/
			std::string getComponentTypeName() const;

			/**
			*  @brief
			*    Return the component type stored in the mime data
			*
			*  @return
			*    Component type name
			*/
			uint32 getComponentType() const;

			/**
			*  @brief
			*    Return the component data as boost ptree stored in the mime data
			*
			*  @param[out] pTree
			*    The boost ptree which will hold the component data
			*/
			void getComponentDataAsBoostPTree(boost::property_tree::ptree& pTree) const;


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
#include "qsf_editor/mimedata/CopyComponentMimeData-inl.h"
