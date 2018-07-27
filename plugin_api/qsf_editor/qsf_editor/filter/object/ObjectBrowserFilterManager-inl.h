// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline ObjectBrowserFilterManager::ObjectBrowserFilterManager(const FilterConfiguration& filterConfiguration) :
			mFilterConfiguration(filterConfiguration)
		{
			// Register all known and supported CAMP pluginable classes
			registerPluginables();
		}

		inline ObjectBrowserFilterManager::~ObjectBrowserFilterManager()
		{
			// Unregister all known and supported CAMP pluginable classes
			unregisterPluginables();
		}

		inline const FilterConfiguration& ObjectBrowserFilterManager::getFilterConfiguration() const
		{
			return mFilterConfiguration;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
