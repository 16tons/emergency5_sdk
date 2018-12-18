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
	*    Target point provider for entering a vehicle, the used door is defined in the vehicle editing
	*/
	class EM5_API_EXPORT EnterVehicleTargetPointProvider : public qsf::logic::TargetPointProvider
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 TARGET_POINT_ID;	///< "EnterVehicleTargetPointProvider" unique identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit EnterVehicleTargetPointProvider(qsf::logic::TargetPointManager* manager);


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EnterVehicleTargetPointProvider)
