// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/logic/action/Action.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class RoadVehicleComponent;
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
	*    EMERGENCY 5 repair vehicle or use object action
	*/
	class EM5_API_EXPORT RepairOrUseAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::RepairOrUseAction" unique action identifier

		enum Posture
		{
			STANDING,
			KNEELING,
			INBULIDING
		};

		enum Target
		{
			VEHICLE,
			OBJECT,	// any object with a UsableByEngineerComponent
			BUILDING
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RepairOrUseAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RepairOrUseAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity, Posture posture, Target target=VEHICLE);

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
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateRepairVehicle(const qsf::Time& timePassed, RoadVehicleComponent& roadVehicleComponent, qsf::Entity& vehicleEntity);

		// NOTE - toolbox is mostly copy-pasted from the medikit stuff in TreatPersonAction. Just found no place to unify it.
		// I tried putting that parts into an own action first, but that makes it harder to guarantee a cleanup as actions can't be
		// inserted in the destructor with the guarantee that they will be called.
		void placeToolbox();
		void takeToolbox();
		void openToolbox() const;
		void closeToolbox() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			INIT,
			STATE_GET_DOWN,
			STATE_OPEN_TOOLBOX,
			START_REPAIRING,
			REPAIRING,
			USING_OBJECT,
			REPARING_BUILDING,
			STATE_CLOSE_TOOLBOX,
			STATE_GET_UP
		};

		Posture				mPosture;
		Target				mTarget;
		State				mState;
		bool				mToolboxPlaced;
		uint64				mTargetEntityId;
		qsf::WeakPtr<qsf::Entity> mToolboxEntity;
		AudioProxy			mAudioProxy;

		qsf::Time mTimeUntilObjectIsUsed;
		qsf::Time mTimeUntilNextRepairStep;
		qsf::Time mTimeUntilRepairSteps;
		float mAmountDamagePerRepairStep;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RepairOrUseAction)
