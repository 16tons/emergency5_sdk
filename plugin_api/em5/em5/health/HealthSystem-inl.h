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
	inline InjuryManager& HealthSystem::getInjuryManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mInjuryManager, "EM5: The injury manager is invalid", QSF_REACT_THROW);
		return *mInjuryManager;
	}

	inline DiagnosisManager& HealthSystem::getDiagnosisManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mDiagnosisManager, "EM5: The diagnosis manager is invalid", QSF_REACT_THROW);
		return *mDiagnosisManager;
	}

	inline MedicalCareManager& HealthSystem::getMedicalCareManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mMedicalCareManager, "EM5: The medical care manager is invalid", QSF_REACT_THROW);
		return *mMedicalCareManager;
	}

	inline bool HealthSystem::getHealthSimulationActive() const
	{
		return mHealthSimulationActive;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* HealthSystem::getName() const
	{
		return "Health system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
