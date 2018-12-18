// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Static editor messages collection class
		*/
		class QSF_EDITOR_API_EXPORT Messages
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    "qsf::editor::PreServerConnection" message; last opportunity to enforce a certain online editor server connection
			*
			*  @remarks
			*    - No filters
			*    - No parameters
			*    - String as response, for example "teamwork.promotion-software.de:4042"
			*/
			static const NamedIdentifier PRE_SERVER_CONNECTION;

			/**
			*  @brief
			*    "qsf::editor::ApplicationStartupFinished" message, after the application finished the startup; Use this to start modding relevant classes here
			*
			*  @remarks
			*    - No filters
			*    - No parameters
			*    - No response
			*/
			static const NamedIdentifier APPLICATION_STARTUP_FINISHED;

			/**
			*  @brief
			*    "qsf::editor::ApplicationShutdownStarting" message, after the application shutdown started; Use this to start modding relevant classes here
			*
			*  @remarks
			*    - No filters
			*    - No parameters
			*    - No response
			*/
			static const NamedIdentifier APPLICATION_SHUTDOWN_STARTING;

			/**
			*  @brief
			*    "PreNewEmptyMap" message, emitted just before the compound operation for the new empty map is pushed
			*
			*  @remarks
			*    - No filters
			*    - Parameters:
			*        "compoundOperation": The compound operation, a "qsf::editor::base::CompoundOperation" pointer
			*    - No response
			*/
			static const qsf::NamedIdentifier PRE_NEW_EMPTY_MAP;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Default constructor
			*/
			Messages();

			/**
			*  @brief
			*    Destructor
			*
			*  @brief
			*    - Not virtual by intent
			*/
			~Messages();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
