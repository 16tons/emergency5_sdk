// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/component/movement/RandomMovementComponent.h>


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
	*    EMERGENCY 5 random movement component class
	*/
	class EM5_API_EXPORT RandomMovementComponent : public qsf::game::RandomMovementComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::RandomMovementComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit RandomMovementComponent(qsf::Prototype* prototype);
		virtual ~RandomMovementComponent();


	//[-----------------------------------------------------------]
	//[ Public virtual qsf::game::RandomMovementComponent methods ]
	//[-----------------------------------------------------------]
	public:
		virtual bool onUpdateSimulation() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::game::RandomMovementComponent methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void pushMoveAction(qsf::ActionComponent& actionComponent, qsf::logic::TargetPoint& moveTargetPoint) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RandomMovementComponent)
