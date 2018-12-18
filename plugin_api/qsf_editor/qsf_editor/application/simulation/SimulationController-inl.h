// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline const SimulationMode* SimulationController::getCurrentSimulationMode() const
		{
			return mCurrentSimulationMode;
		}

		template <typename T>
		void SimulationController::enableSimulationMode()
		{
			T* instance = new T();
			internalEnableSimulationMode(*instance);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
