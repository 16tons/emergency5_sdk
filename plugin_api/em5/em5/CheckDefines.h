// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Check preprocessor defines                            ]
//[-------------------------------------------------------]

#ifndef ENDUSER
	// In case you get this compile error, check your project configuration
	// and add the preprocessor define ENDUSER for all configurations.
	// This define is needed to get the correct binary format for all exports in the SDK.
	#error ENDUSER define not set! Please make sure the ENDUSER define is added to your project settings.
#endif
