// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/base/NamedIdentifier.h>
#include <qsf/time/Time.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>


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
	*    Diagnosis definition class
	*/
	class EM5_API_EXPORT TreatmentInformation : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit TreatmentInformation();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TreatmentInformation();

		// Get the diagnosis ID
		uint32 getDiagnosisId() const;

		// Get the name of the diagnosis
		const std::string& getName() const;

		// Set the name of the diagnosis
		void setName(const std::string& name);

		// Get the time the treatment is used
		const qsf::Time& getUsingTime() const;

		// Set the time of the treatment
		void setUsingTime(const qsf::Time& time);

		const qsf::AssetProxy& getDoctorAnimation() const;
		void setDoctorAnimation(const qsf::AssetProxy& doctorAnimation);

		const qsf::AssetProxy& getDoctorAnimationLoop() const;
		void setDoctorAnimationLoop(const qsf::AssetProxy& doctorAnimationLoop);

		const std::string& getDoctorAnimationEnd() const;
		void setDoctorAnimationEnd(const std::string& doctorAnimationEnd);

		const std::string& getTreatmentLogicName() const;
		void setTreatmentLogicName(const std::string& doctorActionName);

		const std::string& getEquipmentMainName() const;
		void setEquipmentMainName(const std::string& equipmentMainName);

		const std::string& getEquipmentUseAnimationName() const;
		void setEquipmentUseAnimationName(const std::string& equipmentUseAnimationName);

		const std::string& getEquipmentUseLoopAnimationName() const;
		void setEquipmentUseLoopAnimationName(const std::string& equipmentUseLoopAnimationName);

		const std::string& getEquipmentUseEndAnimationName() const;
		void setEquipmentUseEndAnimationName(const std::string& equipmentUseEndAnimationName);

		const std::string& getEquipmentGetAnimationName() const;
		void setEquipmentGetAnimationName(const std::string& equipmentGetAnimationName);

		const std::string& getEquipmentPutBackAnimationName() const;
		void setEquipmentPutBackAnimationName(const std::string& equipmentPutBackAnimationName);

		const std::string& getEquipmentSecondName() const;
		void setEquipmentSecondName(const std::string& equipmentSecondName);

		const qsf::Time& getTime1() const;
		void setTime1(const qsf::Time& time1);

		const qsf::Time& getTime2() const;
		void setTime2(const qsf::Time& time2);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::NamedIdentifier	mName;					///< Name and ID of the diagnosis/medical care
		qsf::Time				mTime;					///< Time how long it takes to make the diagnosis/medical care
		qsf::AssetProxy			mDoctorAnimation;		///< The animation the doctor is playing
		qsf::AssetProxy			mDoctorAnimationLoop;	///< The animation the doctor is looping
		std::string				mDoctorAnimationEnd;	///< The last part of the looped treatment animation
		std::string				mTreatmentLogicName;	///< Name of the action the doctor should play
		std::string				mEquipmentMainName;
		std::string				mEquipmentGetAnimationName;
		std::string				mEquipmentPutBackAnimationName;
		std::string				mEquipmentUseAnimationName;
		std::string				mEquipmentUseLoopedAnimationName;
		std::string				mEquipmentUseEndAnimationName;
		std::string				mEquipmentSecondName;
		qsf::Time				mTime1;
		qsf::Time				mTime2;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::TreatmentInformation)
