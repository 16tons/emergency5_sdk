// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	inline QsfPlugin* QsfPlugin::instance()
	{
		return mGlobalPluginInstance;
	}

	inline QsfPlugin& QsfPlugin::getInstanceSafe()
	{
		QSF_CHECK(mGlobalPluginInstance, "Accessing uninitialized QSF plugin instance",
			QSF_REACT_THROW);

		return *mGlobalPluginInstance;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Plugin methods                    ]
	//[-------------------------------------------------------]
	inline const char* QsfPlugin::getName() const
	{
		return "Quadriga Simulation Framework (QSF)";
	}

	inline const char* QsfPlugin::getVendor() const
	{
		return "Promotion Software GmbH";
	}

	inline const char* QsfPlugin::getDescription() const
	{
		return "Quadriga Simulation Framework (QSF) core plugin";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
