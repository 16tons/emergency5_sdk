// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Material* MaterialManager::getMaterialByOgreIdString(uint32 ogreIdString) const
	{
		OgreIdStringToMaterial::const_iterator iterator = mOgreIdStringToMaterial.find(ogreIdString);
		return (mOgreIdStringToMaterial.cend() != iterator) ? iterator->second : nullptr;
	}

	inline FileCache& MaterialManager::getMaterialFileCache() const
	{
		return mFileCache;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
