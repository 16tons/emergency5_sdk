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
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MeshAnimationChannel;
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
	*    EMERGENCY 5 get equipment action
	*
	*  @note
	*    - This action is used for both getting an equipment from a vehicle and directly switching to an equipment
	*/
	class EM5_API_EXPORT GetEquipmentAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::GetEquipmentAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_TURN_TO_DOOR,
			STATE_OPEN_EQUIPMENT_DOOR,
			STATE_GET_EQUIPMENT,
			STATE_PLAY_USEANIMATION,
			STATE_CLOSE_EQUIPMENT_DOOR,
			STATE_DONE
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		GetEquipmentAction();
		explicit GetEquipmentAction(uint32 identifier);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GetEquipmentAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity, const std::string& equipmentName);

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const std::string& equipmentName, int forceTintColorIndex = -1);

		State getState();

		const std::string& getEquipmentName() const;


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
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void getAndTurnToClosestEquipmentDoor();
		void getAndTurnToClosestEquipmentDoor(bool faceAway, qsf::action::Priority actionPriority);
		DoorComponent* getNearestDoorFromList(const std::vector<DoorComponent*>& doorComponents) const;
		bool checkForNextUserInArea();
		void playUseAnimation();


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GetEquipmentAction methods     ]
	//[-------------------------------------------------------]
	protected:
		virtual std::vector<DoorComponent*> getEquipmentDoorsFromTarget() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		// Configuration
		uint64									mTargetEntityId;		///< ID of the target entity
		std::string								mEquipmentName;			///< Name of the equipment
		DoorComponent::DoorType					mUsedDoorType;			///< Type of the used door
		State									mCurrentState;			///< Current action progress state
		bool									mNeedsTarget;			///< If "true" a target vehicle is needed to get the equipment from, otherwise the equipment may be switched directly
		qsf::WeakPtr<qsf::MeshAnimationChannel>	mUseAnimationChannel;
		bool									mDoctorUsesBackDoor;	///< "true" if the owner is "doctor", "engineer" and "rescuedogleader" and working with the backdoor of the vehicle
		int										mForceTintColorIndex;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::GetEquipmentAction)
