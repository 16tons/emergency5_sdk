// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/base/valueconverter/ValueConverterManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>


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
		*    Abstract value converter provider base class
		*/
		class QSF_EDITOR_API_EXPORT ValueConverterProvider : public Pluginable<ValueConverterManager>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit ValueConverterProvider(ValueConverterManager* valueConverterManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ValueConverterProvider();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::DynamicEnumProvider methods ]
		//[-------------------------------------------------------]
		public:
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
			virtual bool convertQtMimeDataToPropertyValueAsString(const Object& targetObject, int targetPropertyIndex, const QMimeData& qMimeData, std::string& valueAsString) const = 0;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ValueConverterProvider)
