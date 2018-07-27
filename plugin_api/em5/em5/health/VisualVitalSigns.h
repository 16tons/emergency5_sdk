// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/math/BitFlagSet.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/reflection/object/Object.h>
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>


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
	*    EMERGENCY 5 visual vital sign class
	*/
	class EM5_API_EXPORT VisualVitalSigns : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum VisualVitalSignFlags
		{
			// Showing texture layers
			VISUAL_VITAL_SIGN_GUNSHOT_WOUND		= 1<<0,		///< Shows a bloody gunshot wound on the chest
			VISUAL_VITAL_SIGN_BLOOD_COVERED		= 1<<1,		///< Covers the entire body with blood stains
			VISUAL_VITAL_SIGN_BURNED_SKIN		= 1<<2,		///< Shows burned skin over the entire body

			// TODO(tl): Adding a new object
			VISUAL_VITAL_SIGN_BLOOD_PUDDLE		= 1<<3,		///< Adds a blood puddle under the poor person

			// TODO(tl): Altering animation
			VISUAL_VITAL_SIGN_BROKEN_LEG		= 1<<4,		///< Shows a broken leg
			VISUAL_VITAL_SIGN_BROKEN_ARM		= 1<<5,		///< Shows a broken arm
			VISUAL_VITAL_SIGN_COUGHING			= 1<<6,		///< Patient coughing out his lungs
			VISUAL_VITAL_SIGN_VOMITING			= 1<<7,		///< Patient vomiting strange particle effects

			// TODO(tl): Altering mesh
			VISUAL_VITAL_SIGN_AMPUTATED_LEG		= 1<<8,		///< Leg mesh will be missing, maybe you find it elsewhere ?
			VISUAL_VITAL_SIGN_AMPUTATED_ARM		= 1<<9,		///< Arm mesh will be missing

			VISUAL_VITAL_SIGN_BANDAGED_LEG		= 1<<10,	///< Leg mesh is "equipped" with bandaged, an extra mesh with link is attached
			VISUAL_VITAL_SIGN_BANDAGED_ARM		= 1<<11,	///< Arm mesh is "equipped" with bandaged, an extra mesh with link is attached
			VISUAL_VITAL_SIGN_BANDAGED_BODY		= 1<<12,	///< Body mesh is "equipped" with bandaged, an extra mesh with link is attached
		};
		typedef qsf::BitFlagSet<uint16, VisualVitalSignFlags> VisualVitalSignBitmask;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		VisualVitalSigns();

		/**
		*  @brief
		*    Destructor, no virtual by intend
		*/
		~VisualVitalSigns();

		// Getter and Setter Functions for CAMP Export
		bool getVisualVitalSignGunshotWound() const;
		bool getVisualVitalSignBloodCovered() const;
		bool getVisualVitalSignBurnedSkin() const;
		bool getVisualVitalSignBloodPuddle() const;
		bool getVisualVitalSignBrokenLeg() const;
		bool getVisualVitalSignBrokenArm() const;
		bool getVisualVitalSignCoughing() const;
		bool getVisualVitalSignVomitting() const;
		bool getVisualVitalSignAmputatedLeg() const;
		bool getVisualVitalSignAmputatedArm() const;
		bool getVisualVitalSignBandagedLeg() const;
		bool getVisualVitalSignBandagedArm() const;
		bool getVisualVitalSignBandagedBody() const;

		void setVisualVitalSignGunshotWound(bool set);
		void setVisualVitalSignBloodCovered(bool set);
		void setVisualVitalSignBurnedSkin(bool set);
		void setVisualVitalSignBloodPuddle(bool set);
		void setVisualVitalSignBrokenLeg(bool set);
		void setVisualVitalSignBrokenArm(bool set);
		void setVisualVitalSignCoughing(bool set);
		void setVisualVitalSignVomitting(bool set);
		void setVisualVitalSignAmputatedLeg(bool set);
		void setVisualVitalSignAmputatedArm(bool set);
		void setVisualVitalSignBandagedLeg(bool set);
		void setVisualVitalSignBandagedArm(bool set);
		void setVisualVitalSignBandagedBody(bool set);

		void serialize(qsf::BinarySerializer& serializer);

		// Operator
		VisualVitalSigns& operator=(const VisualVitalSigns& other)
		{
			mVisualVitalSignBitmask = other.mVisualVitalSignBitmask;
			return *this;
		}


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		VisualVitalSignBitmask	mVisualVitalSignBitmask;///< What Vital Sign should be shown in a visual way


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::VisualVitalSigns)
