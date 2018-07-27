// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Static EMERGENCY 5 AI navigation map names.
	*    Helps to access the AI navigation maps with EM5 specific symbolic names instead of nondescript numbers
	*/
	class NavigationMap
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		// Always keep this synchronized with the actual values defined inside the editor
		enum Map
		{
			STREET_NETWORK = 0, // available on all maps
			FREE_GROUND_AREA, // available on all maps
			HELICOPTER_VIEW, // available on all maps
			WATER_STREETS, // available on all maps
			DIVER_MAP, // available on all maps
			HAMBURG_SPECIFIC_AFTER_FLOODING, // only available in Hamburg
		};

	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
