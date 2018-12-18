// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/delegate/provider/InjuryDynamicEnumProvider.h"

#include <em5/health/HealthSystem.h>
#include <em5/health/injury/InjuryManager.h>
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
		const std::string InjuryDynamicEnumProvider::SEMANTIC_NAME = "enum:InjuryId";


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		InjuryDynamicEnumProvider::InjuryDynamicEnumProvider(qsf::editor::DynamicEnumManager* dynamicEnumManager) :
			DynamicEnumProvider(dynamicEnumManager)
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::DynamicEnumProvider methods ]
		//[-------------------------------------------------------]
		const std::string& InjuryDynamicEnumProvider::getSemanticName() const
		{
			return SEMANTIC_NAME;
		}

		const qsf::editor::DynamicEnumProvider::ValuePairList& InjuryDynamicEnumProvider::getValues(bool rebuildCachedList)
		{
			if (rebuildCachedList)
			{
				mCachedList.clear();
				InjuryManager& injuryManager = EM5_HEALTH.getInjuryManager();

				// Add standard value
				mCachedList.emplace_back(-1, "healthy");

				for (auto idInjuryPair : injuryManager.getElements())
				{
					const std::string name = idInjuryPair.second->getName();
					mCachedList.emplace_back(QVariant(idInjuryPair.first), name);
				}
			}
			return mCachedList;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
