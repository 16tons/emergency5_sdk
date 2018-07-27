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


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline PrototypeFabricatorManager::PrototypeFabricatorManager(PrototypeManager& prototypeManager) :
			mPrototypeManager(&prototypeManager)
		{
			// Register all known and supported CAMP pluginable classes
			registerPluginables();
		}

		inline PrototypeFabricatorManager::~PrototypeFabricatorManager()
		{
			// Unregister all known and supported CAMP pluginable classes
			unregisterPluginables();
		}

		inline PrototypeManager& PrototypeFabricatorManager::getPrototypeManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mPrototypeManager, "The prototype manager instance is invalid", QSF_REACT_THROW);
			return *mPrototypeManager;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
