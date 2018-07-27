// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor/base/valueconverter/ValueConverterProvider.h>


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
		*    EMERGENCY 5 value converter provider implementation
		*/
		class ValueConverterProvider : public qsf::editor::ValueConverterProvider
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit ValueConverterProvider(qsf::editor::ValueConverterManager* valueConverterManager);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::ValueConverterProvider methods ]
		//[-------------------------------------------------------]
		public:
			virtual bool convertQtMimeDataToPropertyValueAsString(const qsf::Object& targetObject, int targetPropertyIndex, const QMimeData& qMimeData, std::string& valueAsString) const override;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::ValueConverterProvider)
