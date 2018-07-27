// Copyright (C) 2012-2017 Promotion Software GmbH


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
			inline Application::Application() :
				mOperationManager(nullptr),
				mAssetStorageManager(nullptr),
				mNetworkTransferManager(nullptr)
			{
				// Nothing to do in here
			}

			inline Application::~Application()
			{
				// Nothing to do in here
			}

			inline OperationManager& Application::getOperationManager() const
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mOperationManager, "QSF editor base: The operation manager instance is invalid", QSF_REACT_THROW);
				return *mOperationManager;
			}

			inline AssetStorageManager& Application::getAssetStorageManager() const
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mAssetStorageManager, "QSF editor base: The asset storage manager instance is invalid", QSF_REACT_THROW);
				return *mAssetStorageManager;
			}

			inline NetworkTransferManager& Application::getNetworkTransferManager() const
			{
				// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
				QSF_ASSERT(nullptr != mNetworkTransferManager, "QSF editor base: The network transfer manager instance is invalid", QSF_REACT_THROW);
				return *mNetworkTransferManager;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
