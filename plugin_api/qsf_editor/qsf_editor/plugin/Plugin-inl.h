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
	namespace editor
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
			QSF_CHECK(mGlobalPluginInstance, "accessing uninitialized plugin instance",
				QSF_REACT_THROW);

			return *mGlobalPluginInstance;
		}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline Plugin::~Plugin()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Plugin methods                    ]
		//[-------------------------------------------------------]
		inline const char* Plugin::getName() const
		{
			return "QSF Editor";
		}

		inline const char* Plugin::getVendor() const
		{
			return "Promotion Software GmbH";
		}

		inline const char* Plugin::getDescription() const
		{
			return "Quadriga Simulation Framework (QSF) editor";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
