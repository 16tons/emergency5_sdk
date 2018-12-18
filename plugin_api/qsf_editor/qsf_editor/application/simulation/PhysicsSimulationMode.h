// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/application/simulation/SimulationMode.h"


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
		*    Physics simulation mode
		*/
		class PhysicsSimulationMode : public SimulationMode
		{


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::SimulationMode methods    ]
		//[-------------------------------------------------------]
		public:
			virtual void startupSimulation() override;
			virtual void shutdownSimulation() override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
