// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/action/base/ExitVehicleAction.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class HelicopterComponent;
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
	*    EMERGENCY 5 exit from rescue helicopter action
	*/
	class EM5_API_EXPORT ExitRescueHeliAction : public ExitVehicleAction
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::ExitRescueHeliAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ExitRescueHeliAction();
		explicit ExitRescueHeliAction(uint32 actionID);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ExitRescueHeliAction();

		void init(uint64 personIdToExit);


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
		void createOpticalEntities(qsf::Entity& helicopterEntity, bool playAnimationReverse = false);
		void deleteOpticalEntities();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::WeakPtr<HelicopterComponent>	mHelicopterComponent;	///< Pointer to the helicopter component of the owner

		qsf::WeakPtr<qsf::Entity>	mParamedicBackEntity;	///< Is created in action, used only for animation, we have to delete it at the end of the action
		qsf::WeakPtr<qsf::Entity>	mParamedicFrontEntity;	///< Is created in action, used only for animation, we have to delete it at the end of the action
		qsf::WeakPtr<qsf::Entity>	mStretcherEntity;		///< Is created in action, used only for animation, we have to delete it at the end of the action


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*   @brief
		*    Detect what kind of control flow the action has to use
		*/
		void detectStartingPersonal();
		qsf::action::Result updateSubAction(const qsf::Clock& clock);

		// Methods used by the sub action control flow
		void prepareParamedic();
		void setupParamedicForLeaving(qsf::Entity& entity, const std::string& animationName, bool fadeIn, bool playAnimationReverse, const qsf::TransformComponent& helicopterTransformComponent);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		// Defines the control flow of the action in general
		enum GeneralState
		{
			GENREALSTATE_INIT,
			GENREALSTATE_UPDATE,
			GENREALSTATE_DONE
		};

		// Handles the state of the paramedic team
		enum SubState
		{
			STATE_INIT,
			STATE_UPDATE,
			STATE_LINEAR_MOVE,
			STATE_DONE
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		GeneralState	mGeneralState;
		SubState		mSubState;
		bool			mUnloadDoctor;		///< "true" if an "doctor" unit is leaving the RHC, so we have to update the base action "ExitVehicleAction" to unload the doctor correctly
		bool			mUnloadParamedics;	///< "true" if an "Paramedic-team" unit is leaving the RHC, so we have to keep track for our own update cycles
		uint64			mEntityIdToExit;

		// Pointer to units
		qsf::WeakPtr<qsf::Entity>	mParamedicTeam;	///< This is the initial unit, placed in the helicopter (can be a null pointer). Don't delete it.


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ExitRescueHeliAction)
