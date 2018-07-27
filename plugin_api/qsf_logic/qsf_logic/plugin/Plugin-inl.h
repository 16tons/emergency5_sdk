// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
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
		inline SequenceSystem& Plugin::getSequenceSystem() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mSequenceSystem, "The sequence system instance is invalid", QSF_REACT_THROW);
			return *mSequenceSystem;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Plugin methods                    ]
		//[-------------------------------------------------------]
		inline const char* Plugin::getName() const
		{
			return "QSF Logic";
		}

		inline const char* Plugin::getVendor() const
		{
			return "Promotion Software GmbH";
		}

		inline const char* Plugin::getDescription() const
		{
			return "Quadriga Simulation Framework (QSF) logic, contains basic and medium level logic functionality which is shared between multiple projects";
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
	} // logic
} // qsf
