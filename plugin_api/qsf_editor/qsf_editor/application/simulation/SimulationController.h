// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobManager;
	namespace editor
	{
		class SimulationMode;
	}
}


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
		*    Simulation controller class
		*
		*  @remarks
		*    Controls simulation preview inside the editor
		*/
		class SimulationController : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			SimulationController();

			/**
			*  @brief
			*    Destructor
			*/
			~SimulationController();

			/**
			*  @brief
			*    Get the current simulation mode
			*
			*  @return
			*    Pointer to the current simulation mode, or nullptr if none is active
			*/
			inline const SimulationMode* getCurrentSimulationMode() const;

			/**
			*  @brief
			*    Disable the current simulation mode, if any is active
			*/
			void disableSimulationMode();

			/**
			*  @brief
			*    Enable a simulation mode
			*/
			template <typename T>
			void enableSimulationMode();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void disableSimulationJobManagers();
			void enableSimulationJobManagers();

			void internalEnableSimulationMode(SimulationMode& mode);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SimulationMode*			 mCurrentSimulationMode;
			std::vector<JobManager*> mSimulationJobManagers;	///< List of JobManagers that are deactivated while simulation is disabled


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/application/simulation/SimulationController-inl.h"
