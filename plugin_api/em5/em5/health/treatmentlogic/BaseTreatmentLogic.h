// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/logic/gamelogic/GameLogic.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class TreatmentInformation;
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
	*    EMERGENCY 5 base treatment logic
	*    Parent class for treatment process with doctor and target patient
	*/
	class EM5_API_EXPORT BaseTreatmentLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::BaseTreatmentLogic" game logic type ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		BaseTreatmentLogic();
		BaseTreatmentLogic(uint32 gameLogicTypeId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BaseTreatmentLogic();

		/**
		*  @brief
		*    Initialize logic with values
		*/
		void initWithDiagnosis(qsf::Entity& targetEntity, qsf::Entity& doctorEntity, const Diagnosis& diagnosis);
		void initWithMedicalCare(qsf::Entity& targetEntity, qsf::Entity& doctorEntity, const MedicalCare& medicalCare);

		virtual void playFinishAnimation();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	protected:
		// Job update
		virtual void updateSimulation(const qsf::JobArguments& jobArguments);
		virtual qsf::Entity* createEquipment(const std::string& equipmentName, qsf::Entity& parent);
		virtual	void startTreatment();
		virtual bool startLoopedTreatment();
		virtual void pushDoctorBagToTargetAnimation();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;
		qsf::WeakPtr<qsf::Entity>	mDoctorEntity;
		qsf::WeakPtr<qsf::Entity>	mDoctorEquipment;
		TreatmentInformation*		mTreatmentInformation;
		Diagnosis*					mDiagnosis;
		MedicalCare*				mMedicalCare;
		qsf::Time					mUsingTime;
		qsf::Time					mTimeRunning;
		std::string					mDoctorAnimation;
		std::string					mDoctorEquipmentMainName;
		bool						mCanStartTreatment;
		bool						mCanPlayLoopAnimation;
		bool						mCanPlayFinishAnimation;
		bool						mDoctorCanStandUp;
		qsf::JobProxy				mSimulationJobProxy;
		AudioProxy					mAudioProxy;


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
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::BaseTreatmentLogic)
