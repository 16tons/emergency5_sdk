// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline FireMaterialManager& FireSystem::getFireMaterialManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mFireMaterialManager, "EM5: The Fire material manager is invalid", QSF_REACT_THROW);
		return *mFireMaterialManager;
	}

	inline bool FireSystem::getFireSimulationActive() const
	{
		return mFireSimulationEnabled;
	}

	inline void FireSystem::resetFireSystem()
	{
		mResetFireSystem = true;
	}

	inline FireSystem::SimulationVisualisationMode FireSystem::getFireRadiansSimulationVisualisationMode() const
	{
		return mFireRadiansVisualisationMode;
	}

	inline void FireSystem::setFireRadiansSimulationVisualisationMode(FireSystem::SimulationVisualisationMode visualisationMode)
	{
		mFireRadiansVisualisationMode = visualisationMode;
	}

	inline FireSystem::SimulationVisualisationMode FireSystem::getFireSpreadSimulationVisualisationMode() const
	{
		return mFireSpreadVisualisationMode;
	}

	inline void FireSystem::setFireSpreadSimulationVisualisationMode(FireSystem::SimulationVisualisationMode visualisationMode)
	{
		mFireSpreadVisualisationMode = visualisationMode;
	}

	inline FireSystem::FireSpreadLinesVisualisationMode FireSystem::getFireSpreadLinesVisualisationMode() const
	{
		return mFireSpreadLinesVisualisationMode;
	}

	inline void FireSystem::setFireSpreadLinesVisualisationMode(FireSystem::FireSpreadLinesVisualisationMode visualisationMode)
	{
		mFireSpreadLinesVisualisationMode = visualisationMode;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* FireSystem::getName() const
	{
		return "Fire system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
