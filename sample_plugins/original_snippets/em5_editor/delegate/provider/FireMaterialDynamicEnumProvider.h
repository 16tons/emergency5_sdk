// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor/delegate/provider/DynamicEnumProvider.h>


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
		*    Fire material dynamic enum provider class, knows how to display enums with "Semantic" = "enum:FireMaterial"
		*/
		class FireMaterialDynamicEnumProvider : public qsf::editor::DynamicEnumProvider
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const std::string SEMANTIC_NAME;	///< Enum semantic name "enum:FireMaterial"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit FireMaterialDynamicEnumProvider(qsf::editor::DynamicEnumManager* dynamicEnumManager);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::DynamicEnumProvider methods ]
		//[-------------------------------------------------------]
		public:
			virtual const std::string& getSemanticName() const override;
			virtual const ValuePairList& getValues(bool rebuildCachedList) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::FireMaterialDynamicEnumProvider)