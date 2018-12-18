// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"

#include <qsf/application/TimedApplication.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class OperationManager;
			class AssetStorageManager;
			class NetworkTransferManager;
		}
	}
}


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
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    QSF editor base application implementation
			*/
			class QSF_EDITOR_BASE_API_EXPORT Application : public TimedApplication
			{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			public:
				/**
				*  @brief
				*    Default constructor
				*/
				inline Application();

				/**
				*  @brief
				*    Destructor
				*/
				inline virtual ~Application();

				/**
				*  @brief
				*    Return the operation manager instance
				*
				*  @return
				*    The operation manager instance, do not destroy the returned instance
				*/
				inline OperationManager& getOperationManager() const;

				/**
				*  @brief
				*    Return the asset storage manager instance
				*
				*  @return
				*    Asset storage manager instance, do not destroy the instance
				*/
				inline AssetStorageManager& getAssetStorageManager() const;

				/**
				*  @brief
				*    Return the network transfer manager instance
				*
				*  @return
				*    Network transfer manager instance, do not destroy the instance
				*/
				inline NetworkTransferManager& getNetworkTransferManager() const;


			//[-------------------------------------------------------]
			//[ Protected data                                        ]
			//[-------------------------------------------------------]
			protected:
				OperationManager*		mOperationManager;			///< Operation manager instance, always valid while the application is running, we have to destroy the instance in case we no longer need it
				AssetStorageManager*	mAssetStorageManager;		///< Asset storage manager instance, always valid while the application is running, we have to destroy the instance in case we no longer need it
				NetworkTransferManager*	mNetworkTransferManager;	///< Network transfer manager instance, always valid while the application is running, we have to destroy the instance in case we no longer need it


			//[-------------------------------------------------------]
			//[ CAMP reflection system                                ]
			//[-------------------------------------------------------]
			QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor_base/application/Application-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::base::Application)
