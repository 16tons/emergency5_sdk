// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/delegate/provider/DynamicEnumProvider.h"

#include <unordered_set>


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
		*    UI style sheet dynamic enum provider class, knows how to display enums with "Semantic" = "enum:UiStyleSheet"
		*/
		class QSF_EDITOR_API_EXPORT UiStyleSheetDynamicEnumProvider : public DynamicEnumProvider
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32		 PLUGINABLE_ID;	///< "qsf::editor::UiStyleSheetDynamicEnumProvider" unique pluginable dialog ID
			static const std::string SEMANTIC_NAME;	///< Enum semantic name "enum:UiStyleSheet"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit UiStyleSheetDynamicEnumProvider(DynamicEnumManager* dynamicEnumManager);

			/**
			*  @brief
			*    Add a UI style sheets directory
			*
			*  @param[in] uiStyleSheetsDirectory
			*    UI style sheets directory to add, for example "../../data/qsf/editor/style_sheets/"
			*
			*  @note
			*    - Duplicates are detected automatically
			*/
			void addUiStyleSheetsDirectory(const std::string& uiStyleSheetsDirectory);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::DynamicEnumProvider methods ]
		//[-------------------------------------------------------]
		public:
			virtual const std::string& getSemanticName() const override;
			virtual const DynamicEnumManager::ValuePairList& getValues(bool rebuildCachedList) override;


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::unordered_set<std::string> UiStyleSheetsDirectory;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			UiStyleSheetsDirectory mUiStyleSheetsDirectory;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::UiStyleSheetDynamicEnumProvider)
