// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/action/ambulance/ExitRescueHeliAction.h"


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
	*    EMERGENCY 5 enter rescue helicopter with paramedic action
	*
	*  @remarks
	*    The action is owned by an paramedic team, entering the helicopter. The owner of base class "em5::ExitRescueHeliAction" is performed on the helicopter.
	*/
	class EM5_API_EXPORT EnterRescueHeliAction : public ExitRescueHeliAction
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::EnterRescueHeliAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_ENTER,
			STATE_CLOSE_TRUNK_DOOR,
			STATE_UPDATE,
			STATE_CLOSE_PASSENGER_DOOR,
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
		EnterRescueHeliAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EnterRescueHeliAction();

		void init(qsf::Entity& targetEntity);
		uint64 getTargetId() const;


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
		void prepareParamedicForEnter();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State						mState;
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;			///< Points to the helicopter we want to enter. Do not delete.
		qsf::Time					mTimeTillDoorsClosed;
		qsf::WeakPtr<qsf::Entity>	mPatientEntity;			///< Points to the patient we carry, can be a null pointer


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EnterRescueHeliAction)
