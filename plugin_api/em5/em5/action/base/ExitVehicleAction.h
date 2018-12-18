// Copyright (C) 2012-2018 Promotion Software GmbH


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
namespace em5
{
	class VehicleComponent;
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
	*    EMERGENCY 5 exit vehicle action
	*/
	class EM5_API_EXPORT ExitVehicleAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::ExitVehicleAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ExitVehicleAction();

		explicit ExitVehicleAction(uint32 actionId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ExitVehicleAction();

		/**
		*  @brief
		*    Give the ID of the target how exits the vehicle, if empty (or not called) then everybody exits the vehicle
		*/
		void init(uint64 entityId);

		/**
		*  @brief
		*    Use this initialization to unload everyone inside, even injured persons
		*/
		void initForUnloadUnfiltered();


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
		void findCorrectVehicleDoor();
		void exitEntityFromVehicle();
		void getPlaceTransform(qsf::Transform& outTransform);
		float getPlaceOffset(int index);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_OPEN_DOOR,
			STATE_EXIT,
			STATE_WAIT_FOR_PERSONAL,
			STATE_CLOSE_DOOR
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State							mCurrentState;
		DoorComponent::DoorType			mDoorType;			///< Identifies the door to use
		qsf::WeakPtr<VehicleComponent>	mVehicleComponent;
		qsf::Time						mWaitTillPersonsFadedIn;
		std::vector<qsf::Entity*>		mEntitiesToExit;
		bool							mSelectPersonsAfterExitVehicle;
		uint64							mExitPersonId;		///< Entity ID of the person how leaves the vehicle, if set, only the entity exits the vehicle
		bool							mUnloadUnfiltered;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ExitVehicleAction)
