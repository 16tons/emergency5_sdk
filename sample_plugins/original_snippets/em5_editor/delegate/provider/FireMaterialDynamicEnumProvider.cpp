// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/delegate/provider/FireMaterialDynamicEnumProvider.h"

#include <em5/fire/FireSystem.h>
#include <em5/fire/FireMaterialManager.h>
#include <em5/EM5Helper.h>


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
		const std::string FireMaterialDynamicEnumProvider::SEMANTIC_NAME = "enum:FireMaterial";


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		FireMaterialDynamicEnumProvider::FireMaterialDynamicEnumProvider(qsf::editor::DynamicEnumManager* dynamicEnumManager) :
			DynamicEnumProvider(dynamicEnumManager)
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::DynamicEnumProvider methods ]
		//[-------------------------------------------------------]
		const std::string& FireMaterialDynamicEnumProvider::getSemanticName() const
		{
			return SEMANTIC_NAME;
		}

		const qsf::editor::DynamicEnumProvider::ValuePairList& FireMaterialDynamicEnumProvider::getValues(bool rebuildCachedList)
		{
			if (rebuildCachedList)
			{
				mCachedList.clear();

				for (auto idMaterialPair : EM5_FIRE.getFireMaterialManager().getElements())
				{
					const std::string name = idMaterialPair.second->getName();
					mCachedList.emplace_back(QVariant(name.c_str()), name);
				}

				// Sort by fire material name
				std::sort(mCachedList.begin(), mCachedList.end(),
					[](qsf::editor::DynamicEnumManager::ValuePair& a, qsf::editor::DynamicEnumManager::ValuePair& b) { return a.second < b.second; }
				);
			}
			return mCachedList;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
