// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    OpenVR runtime linking
	*/
	class OpenVrRuntimeLinking
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class VrManagerOpenVrWin32;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		OpenVrRuntimeLinking();

		/**
		*  @brief
		*    Destructor
		*/
		~OpenVrRuntimeLinking();

		/**
		*  @brief
		*    Return whether or not OpenVR is available
		*
		*  @return
		*    "true" if OpenVR is available, else "false"
		*/
		bool isOpenVrAvaiable();

		/**
		*  @brief
		*    Load the shared libraries
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool loadSharedLibraries();

		/**
		*  @brief
		*    Load the OpenVR entry points
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool loadOpenVrEntryPoints();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		void* mOpenVrSharedLibrary;		///< OpenVR shared library, can be a null pointer
		bool  mEntryPointsRegistered;	///< Entry points successfully registered?
		bool  mInitialized;				///< Already initialized?


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
