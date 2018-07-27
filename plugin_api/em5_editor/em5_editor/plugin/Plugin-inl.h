// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
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
			QSF_CHECK(mGlobalPluginInstance, "Accessing uninitialized EM5 editor plugin instance",
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
			return "EMERGENCY 5 (EM5) Editor";
		}

		inline const char* Plugin::getVendor() const
		{
			return "Promotion Software GmbH";
		}

		inline const char* Plugin::getDescription() const
		{
			return "EMERGENCY 5 (EM5) editor";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5
