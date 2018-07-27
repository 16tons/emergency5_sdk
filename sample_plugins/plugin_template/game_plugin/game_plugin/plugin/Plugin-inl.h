// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace user
{


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
		return "EMERGENCY 5 user-defined plugin";	// Please replace with a better name
	}

	inline const char* Plugin::getVendor() const
	{
		return "Unknown";	// Replace with your own name as author
	}

	inline const char* Plugin::getDescription() const
	{
		return "User-defined plugin description";	// Insert a plugin description here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // user
