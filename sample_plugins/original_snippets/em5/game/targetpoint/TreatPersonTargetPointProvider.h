// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_logic/targetpoint/TargetPointProvider.h>


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
	*    Target point provider for treat a person
	*/
	class EM5_API_EXPORT TreatPersonTargetPointProvider : public qsf::logic::TargetPointProvider
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 TARGET_POINT_ID;	///< "TreatPersonTargetPointProvider" unique identifier

		static const float TargetPersonXOffset;
		static const float TargetPersonZOffsetMale;
		static const float TargetPersonZOffsetFemale;
		static const float TargetPersonXOffsetStanding;
		static const float TargetPersonZOffsetStanding;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit TreatPersonTargetPointProvider(qsf::logic::TargetPointManager* manager);

		static void calculateTargetPoints_static(qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints, bool originalSide = true);
		static glm::quat calculateRotation_static(const qsf::Transform& targetTransform, bool originalSide);

		static void calculateOffsets(qsf::Entity& target, float& xOffset, float& zOffset);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::logic::TargetPointProvider methods ]
	//[-------------------------------------------------------]
	public:
		virtual void calculateTargetPoints(qsf::Entity& caller, const qsf::Transform& callerTransform, qsf::Entity& target, const qsf::Transform& targetTransform, std::vector<qsf::logic::TargetPoint>& targetPoints) const override;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::TreatPersonTargetPointProvider)
