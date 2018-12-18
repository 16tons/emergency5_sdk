// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline TextureSemanticManager& AssetStorageManager::getTextureSemanticManager()
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mTextureSemanticManager, "The texture semantic manager instance is invalid", QSF_REACT_THROW);
				return *mTextureSemanticManager;
			}

			inline const TextureSemanticManager& AssetStorageManager::getTextureSemanticManager() const
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mTextureSemanticManager, "The texture semantic manager instance is invalid", QSF_REACT_THROW);
				return *mTextureSemanticManager;
			}

			inline AssetMaintenanceManager& AssetStorageManager::getAssetMaintenanceManager()
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mAssetMaintenanceManager, "The asset maintenance manager instance is invalid", QSF_REACT_THROW);
				return *mAssetMaintenanceManager;
			}

			inline const AssetMaintenanceManager& AssetStorageManager::getAssetMaintenanceManager() const
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mAssetMaintenanceManager, "The asset maintenance manager instance is invalid", QSF_REACT_THROW);
				return *mAssetMaintenanceManager;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
