// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/door/DoorComponent.h"

#include <qsf/logic/action/Action.h>


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
	*    EMERGENCY 5 enter vehicle action
	*/
	class EM5_API_EXPORT EnterVehicleAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::EnterVehicleAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool isInDoorRange(qsf::Entity& callerEntity, qsf::Entity& targetEntity, DoorComponent::DoorType doorType, float maxDistance);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		EnterVehicleAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EnterVehicleAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity, DoorComponent::DoorType doorType = DoorComponent::DOORTYPE_UNDEFINED);

		uint64 getTargetId() const;
		DoorComponent::DoorType getTargetDoorType() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual bool onStartup() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool checkActionCondition();
		bool checkForNextUserInArea(qsf::Entity& targetEntity) const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_OPEN_DOOR,
			STATE_ENTER,
			STATE_IS_ENTERED,
			STATE_CLOSE_DOOR
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64					mTargetEntityId;	///< Vehicle entity ID, must be valid
		DoorComponent::DoorType	mDoorType;			///< Identifies the door to use

		// Internal
		State					mCurrentState;		///< Current action progress state
		qsf::Time				mWaitTime;			///< Wait time for timed states
		qsf::WeakPtr<qsf::MeshAnimationChannel>	mMeshAnimationChannel;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EnterVehicleAction)
