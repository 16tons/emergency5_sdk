// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/System.h>
#include <qsf/reflection/CampClass.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class StringHash;
}
namespace em5
{
	class DiagnosisManager;
	class InjuryManager;
	class MedicalCareManager;
	class Injury;
	class Diagnosis;
	class MedicalCare;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Health system
	*/
	class EM5_API_EXPORT HealthSystem : public qsf::System
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HealthSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HealthSystem();

		// Get injury by ID
		const Injury* getInjuryById(const qsf::StringHash& id) const;

		// Get diagnosis by ID
		const Diagnosis* getDiagnosisById(const qsf::StringHash& id) const;

		// Get medical care by ID
		const MedicalCare* getMedicalCareById(const qsf::StringHash& id) const;

		/**
		*  @brief
		*    Get the injury manager instance, do not delete it
		*/
		inline InjuryManager& getInjuryManager() const;

		/**
		*  @brief
		*    Get the diagnosis manager instance, do not delete it
		*/
		inline DiagnosisManager& getDiagnosisManager() const;

		/**
		*  @brief
		*    Get the medical care manager instance, do not delete it
		*/
		inline MedicalCareManager& getMedicalCareManager() const;

		inline bool getHealthSimulationActive() const;
		void setHealthSimulationActive(bool healthSimulationEnabled);

		float getEnergyLossMultiplier() const;
		void setEnergyLossMultiplier(float multiplier);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		DiagnosisManager*	mDiagnosisManager;		///< Diagnosis manager; always valid, we're responsible for destroying the instance
		InjuryManager*		mInjuryManager;			///< Injury manager; always valid, we're responsible for destroying the instance
		MedicalCareManager*	mMedicalCareManager;	///< Medicalcare manager; always valid, we're responsible for destroying the instance
		bool				mHealthSimulationActive;///< If "false", whole health simulation including healing is paused
		float				mEnergyLossMultiplier;	///< Usually 1.0f, or set to a different non-negative value to increase or decrease energy loss due to injuries


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/health/HealthSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::HealthSystem)
