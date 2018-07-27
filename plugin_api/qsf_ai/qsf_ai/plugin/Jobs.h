// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"

#include <qsf/job/Jobs.h>
#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Static QSF AI plugin jobs collection class
		*/
		class QSF_AI_API_EXPORT Jobs : public qsf::Jobs
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class Plugin;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Simulation
			static const NamedIdentifier SIMULATION_AI;		///< Job manager "Simulation.Ai"


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Default constructor
			*/
			Jobs();

			/**
			*  @brief
			*    Destructor
			*
			*  @brief
			*    - Not virtual by intent
			*/
			~Jobs();

			/**
			*  @brief
			*    Register the job managers
			*/
			static void registerJobManagers();

			/**
			*  @brief
			*    Unregister the job managers
			*/
			static void unregisterJobManagers();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
