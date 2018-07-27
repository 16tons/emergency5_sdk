// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/VisualVitalSigns.h"
#include "em5/reflection/type/CampDiagnosisIdentifier.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/math/BitFlagSet.h>
#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Diagnosis;
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
	*    Injury definition class
	*/
	class EM5_API_EXPORT Injury : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class InjuryManager;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/** Injury flags */
		enum InjuryFlags
		{
			INJURY_AFFECTS_HEALTH = 1<<0,	///< Injury affects health instead of life (e.g. "healthy" parts of illnesses and contaminations)
			INJURY_SPREADS		  = 1<<1	///< Injury spreads to other people
		};
		typedef qsf::BitFlagSet<uint8, InjuryFlags> InjuryBitmask;

		/** Contamination type */
		enum ContaminationType
		{
			CONTAMINATION_NONE		  = 0,	///< No contamination
			CONTAMINATION_RADIOACTIVE = 1,	///< Radioactive contamination
			CONTAMINATION_CHEMICAL	  = 2,	///< Chemical contamination
			CONTAMINATION_PLAGUE	  = 3,	///< Plague contamination
			CONTAMINATION_BIOLOGICAL  = 4	///< Biological contamination
		};

		typedef std::vector<uint32> MedicalCareIdArray;
		typedef std::vector<uint32> InjuryIdArray;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit Injury(uint32 key);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Injury();

		// Get the injury ID
		uint32 getInjuryId() const;

		// Get the name of the injury
		const std::string& getName() const;

		// Set the name of the injury
		void setName(const std::string& name);

		// Get the localized name of the injury
		const std::string& getLocalizedName() const;

		// Set the localized name of the injury
		void setLocalizedName(const std::string& name);

		// Get the injury bitmask
		InjuryBitmask getInjuryBitmask() const;

		// Get the visual vital signs
		const VisualVitalSigns& getVisualVitalSigns() const;

		// Get injury initial reduction factor
		float getInitialReduction() const;

		// Set injury initial reduction factor
		void setInitialReduction(float initialReduction);

		// Get the injury energy delta
		float getInjuryDelta() const;

		// Set the injury energy delta
		void setInjuryDelta(float injuryDelta);

		// Get the heal delta
		float getHealDelta() const;

		// Set the heal delta
		void setHealDelta(float healDelta);

		// Get treatment threshold
		float getTreatmentThreshold() const;

		// Set treatment threshold
		void setTreatmentThreshold(float treatmentThreshold);

		// Get treatment factor
		float getTreatmentFactor() const;

		// Set treatment factor
		void setTreatmentFactor(float treatmentFactor);

		// Get the diagnosis ID
		DiagnosisIdentifier getDiagnosisId() const;

		// Set the diagnosis ID
		void setDiagnosisId(DiagnosisIdentifier diagnosisId);

		// Get the diagnosis
		const Diagnosis* getDiagnosis() const;

		// Set the name of the injury animation
		void setAnimationName(const std::string& animationName);

		// Get the name of the injury animation
		const std::string& getAnimationName() const;

		// Get the name of the audio file without path and person type
		const std::string& getAudioName() const;

		// Set the name of the audio file without path and person type
		void setAudioName(const std::string& audioName);

		//[-------------------------------------------------------]
		//[ Contaminations & illnesses                            ]
		//[-------------------------------------------------------]
		// Return whether the injury is affecting health instead of life
		bool isAffectingHealth() const;

		// Set whether the injury is affecting health instead of life
		void setAffectsHealth(bool affectsHealth);

		// Get contamination type
		ContaminationType getContaminationType() const;

		// Return whether this injury is a contamination
		bool isContamination() const;

		// Set contamination type
		void setContaminationType(ContaminationType contaminationType);

		// Get spread delay
		float getSpreadDelay() const;

		// Set spread delay
		void setSpreadDelay(float spreadDelay);

		// Get the spread chance
		float getSpreadChance() const;

		// Set the spread chance
		void setSpreadChance(float spreadChance);

		// Get spread radius
		float getSpreadRadius() const;

		// Set spread radius
		void setSpreadRadius(float spreadRadius);

		// Get the name of the spreaded injury
		const std::string& getSpreadedInjuryName() const;

		// Set the name of the spreaded injury
		void setSpreadedInjuryName(const std::string& injuryName);

		// Get the injury to switch to when the person's energy falls to zero
		const InjuryIdArray& getFollowingInjuries() const;

		// Get if the injury is drowning
		bool getIsDrowning() const;

		// Set if the injury is drowning
		void setIsDrowing(bool isDrowning);

		// "true" if the target is not standing, not lying
		bool getIsKneeing() const;
		void setIsKneeing(bool isKneeing);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// General definitions
		qsf::NamedIdentifier	mName;					///< Injury name; must be unique
		std::string				mLocalizedName;			///< Injury localized name ID
		InjuryBitmask			mInjuryBitmask;			///< What kind of injury is this, to avoid unnecessary checks
		VisualVitalSigns		mVisualVitalSigns;		///< What vital sign should be shown in a visual way
		float					mInitialReduction;		///< Life/health reduction when the person gets injured, inside interval 0.0f..1.0f
		float					mInjuryDelta;			///< Life/health reduction rate per second before the patient was healed at least to the "threshold" value
		float					mHealDelta;				///< Life/health increase rate per second while the patient is treated by a doctor
		float					mTreatmentThreshold;	///< Threshold value for life/health energy above which another injury delta is applied, inside interval 0.0f..1.0f
		float					mTreatmentFactor;		///< Factor that is multiplied to the current injury delta, after the person was healed at least to the "threshold" value, inside interval 0.0f..1.0f
		DiagnosisIdentifier		mDiagnosisId;			///< ID of the diagnosis to apply for this injury
		MedicalCareIdArray		mMedicalCareIds;		///< IDs of the medical care steps to apply for this injury
		std::string				mAnimationName;			///< Name (with path) the injury animation asset
		std::string				mAudioName;				///< Name of the audio file without path and person type, the person is starting in random time interval
		bool					mIsKneeing;				///< "true" if target is kneeing (de: knieend), needs special treatment

		// For contaminations and illnesses
		ContaminationType		mContaminationType;		///< Type of contamination
		float					mSpreadDelay;			///< Delay in which the spreading injury is hidden from the player
		float					mSpreadRadius;			///< Radius in which the injury spreads
		float					mSpreadChance;			///< Chance that a person within the spread radius is injured, inside interval 0.0f..1.0f
		std::string				mSpreadedInjuryName;	///< The name of the injury what is spreaded (used to spread the "main" injury, not the _lying part)
		InjuryIdArray			mFollowingInjuries;		///< IDs of the possible injuries to switch to when the person's energy falls to zero
		bool					mIsDrowning;			///< "true" if the person is in or underwater


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(MedicalCareIdArrayInterface, Injury, std::vector<uint32>, std::string);
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(InjuryIdArrayInterface, Injury, std::vector<uint32>, std::string);

		MedicalCareIdArrayInterface	MedicalCareArray;		///< Array of the medical care steps
		InjuryIdArrayInterface		FollowingInjuryArray;	///< Array of the possible following injuries


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::Injury)
QSF_CAMP_TYPE_NONCOPYABLE(em5::Injury::ContaminationType)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::Injury::MedicalCareIdArrayInterface)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::Injury::InjuryIdArrayInterface)
