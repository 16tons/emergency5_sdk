// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/plugin/pluginable/PluginableManager.h>

#include <QtCore/qglobal.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QMimeData;
QT_END_NAMESPACE
namespace qsf
{
	class Object;
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
		*    Value converter manager implementation
		*
		*  @remarks
		*    The value converter manager is automatically able to use all CAMP classes which are derived from "qsf::editor::ValueConverterProvider".
		*/
		class QSF_EDITOR_API_EXPORT ValueConverterManager : public PluginableManager
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			ValueConverterManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ValueConverterManager();

			/**
			*  @brief
			*    Convert Qt mime data to property value as string
			*
			*  @param[in] targetObject
			*    Target object
			*  @param[in] targetPropertyIndex
			*    Target property index
			*  @param[in] qMimeData
			*    Qt mime data
			*  @param[out] valueAsString
			*    Resulting property value as string
			*
			*  @return
			*    "true" if the conversion was successful, else "false"
			*/
			bool convertQtMimeDataToPropertyValueAsString(const Object& targetObject, int targetPropertyIndex, const QMimeData& qMimeData, std::string& valueAsString) const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::PluginableManager methods      ]
		//[-------------------------------------------------------]
		protected:
			virtual void classAdded(const camp::Class& added) override;
			virtual void createPluginableInstance(const camp::Class& added, camp::UserObject& campUserObject) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ValueConverterManager)
