// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5_editor/PrecompiledHeader.h"
#include "em5_editor/delegate/provider/MoverTypeDynamicEnumProvider.h"


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
		const std::string MoverTypeDynamicEnumProvider::SEMANTIC_NAME = "enum:MoverType";


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		MoverTypeDynamicEnumProvider::MoverTypeDynamicEnumProvider(qsf::editor::DynamicEnumManager* dynamicEnumManager) :
			DynamicEnumProvider(dynamicEnumManager)
		{
			// Nothing here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::DynamicEnumProvider methods ]
		//[-------------------------------------------------------]
		const std::string& MoverTypeDynamicEnumProvider::getSemanticName() const
		{
			return SEMANTIC_NAME;
		}

		const qsf::editor::DynamicEnumManager::ValuePairList& MoverTypeDynamicEnumProvider::getValues(bool rebuildCachedList)
		{
			if (rebuildCachedList)
			{
				mCachedList.clear();

				// TODO(mk) monster hack. i review this asp, move it to em5, but for now it should be ok
				// TODO(co) What about localization? Required?
				mCachedList.emplace_back(QVariant(0), "player vehicle");
				mCachedList.emplace_back(QVariant(1), "player unit");
				mCachedList.emplace_back(QVariant(2), "civil vehicle");
				mCachedList.emplace_back(QVariant(3), "civil person");
				mCachedList.emplace_back(QVariant(4), "gangster in car");
				mCachedList.emplace_back(QVariant(5), "gangster person");
				mCachedList.emplace_back(QVariant(6), "player ship");
				mCachedList.emplace_back(QVariant(7), "flying vehicle");
				mCachedList.emplace_back(QVariant(8), "train or tram");
				mCachedList.emplace_back(QVariant(9), "player boat");
				mCachedList.emplace_back(QVariant(10), "civil ship");
			}
			return mCachedList;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
