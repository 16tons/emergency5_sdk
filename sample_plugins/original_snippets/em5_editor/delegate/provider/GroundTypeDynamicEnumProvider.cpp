// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/delegate/provider/GroundTypeDynamicEnumProvider.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		const std::string GroundTypeDynamicEnumProvider::SEMANTIC_NAME = "enum:GroundType";


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		GroundTypeDynamicEnumProvider::GroundTypeDynamicEnumProvider(qsf::editor::DynamicEnumManager* dynamicEnumManager) :
			DynamicEnumProvider(dynamicEnumManager)
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::DynamicEnumProvider methods ]
		//[-------------------------------------------------------]
		const std::string& GroundTypeDynamicEnumProvider::getSemanticName() const
		{
			return SEMANTIC_NAME;
		}

		const qsf::editor::DynamicEnumManager::ValuePairList& GroundTypeDynamicEnumProvider::getValues(bool rebuildCachedList)
		{
			if (rebuildCachedList)
			{
				mCachedList.clear();

				// TODO(oi) Is there a need to have a more dynamic ground type database? Then the data source needs to be changed here
				mCachedList.emplace_back(QVariant(0), "default");
				mCachedList.emplace_back(QVariant(1), "with footsteps and tracks");
			}
			return mCachedList;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
