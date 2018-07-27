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
	*    EMERGENCY 5 place person action
	*/
	class EM5_API_EXPORT PlacePersonAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::PlacePersonAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_OPEN_DOOR,
			STATE_ENTER_VEHICLE,
			STATE_PLACE_PERSON,
			STATE_CLOSE_DOOR,
			STATE_DONE
		};

		enum PlaceCase
		{
			PLACECASE_UNKNOWN,
			PLACECASE_PATIENT,	///< We want to place a patient inside an ambulance vehicle
			PLACECASE_GANGSTER,	///< We want to place a gangster inside a police vehicle
			PLACECASE_CIVILIAN	///< We want to place a non-injured civilian inside an evacuation vehicle
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void lookForIdleMedicsInArea(qsf::Entity& vehicleEntity);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PlacePersonAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PlacePersonAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(qsf::Entity& targetEntity, bool placeGuidedPerson = false);

		uint64 getTargetEntityId() const;


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
		bool checkPlacementStyle(qsf::Entity& vehicleEntity);
		qsf::Entity* findClosestDoor(qsf::Entity& vehicleEntity);
		void showAutoTransportHint() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64	mTargetEntityId;
		bool	mPlaceGuidedPerson;

		// Internal
		State						mCurrentState;	///< Current action progress state
		PlaceCase					mPlaceCase;		///< What kind of placement action this is
		DoorComponent::DoorType		mDoorType;
		qsf::WeakPtr<qsf::Entity>	mDoorEntity;	///< Door we want to use to place the person

		qsf::Time	mDoorAnimationWatchDog;	///< Extra timing value for checking if something is wrong with the doors, in case watch dog reaches timeout, skip the waiting


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PlacePersonAction)
