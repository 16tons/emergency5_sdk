// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		inline Plugin* Plugin::instance()
		{
			return mGlobalPluginInstance;
		}

		inline Plugin& Plugin::getInstanceSafe()
		{
			QSF_CHECK(mGlobalPluginInstance, "Accessing uninitialized plugin instance", QSF_REACT_THROW);
			return *mGlobalPluginInstance;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Plugin methods                    ]
		//[-------------------------------------------------------]
		inline CompositorSystem& Plugin::getCompositorSystem() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mCompositorSystem, "The compositor system instance is invalid", QSF_REACT_THROW);
			return *mCompositorSystem;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Plugin methods                    ]
		//[-------------------------------------------------------]
		inline const char* Plugin::getName() const
		{
			return "QSF Compositing";
		}

		inline const char* Plugin::getVendor() const
		{
			return "Promotion Software GmbH";
		}

		inline const char* Plugin::getDescription() const
		{
			return "Quadriga Simulation Framework (QSF) compositing, contains high level functionality which is shared between multiple projects";
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::SystemContainer methods           ]
		//[-------------------------------------------------------]
		const char* Plugin::getSystemContainerName() const
		{
			return getName();
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf
