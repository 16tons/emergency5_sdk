// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


// This file is used only for very central definitions that can't easily placed anywhere else
//  -> Don't ever put anything here, except it is authorized by your lead programmer
//  -> Currently unused definitions can be found inside "VersionConfiguration.unused" (not part of the SDK)


//[-------------------------------------------------------]
//[ Version settings                                      ]
//[-------------------------------------------------------]

// End-user version switch
//  -> Before building a final master, be sure to enable this
//  -> This should remove all kinds of debug menus and output from the application
#ifdef RETAIL
	//#define ENDUSER
#endif
