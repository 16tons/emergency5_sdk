// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline IMaterialBlueprintListener& MaterialBlueprintManager::getMaterialBlueprintListener() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMaterialBlueprintListener, "The material blueprint listener instance is invalid", QSF_REACT_THROW);
		return *mMaterialBlueprintListener;
	}

	inline MaterialProperties& MaterialBlueprintManager::getGlobalMaterialProperties()
	{
		return mGlobalMaterialProperties;
	}

	inline const MaterialProperties& MaterialBlueprintManager::getGlobalMaterialProperties() const
	{
		return mGlobalMaterialProperties;
	}

	inline MaterialBlueprint* MaterialBlueprintManager::getMaterialBlueprintByOgreIdString(uint32 ogreIdString) const
	{
		OgreIdStringToMaterialBlueprint::const_iterator iterator = mOgreIdStringToMaterialBlueprint.find(ogreIdString);
		return (mOgreIdStringToMaterialBlueprint.cend() != iterator) ? iterator->second : nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
