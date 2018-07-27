// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/treatmentlogic/BaseTreatmentLogic.h"


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
	*    EMERGENCY 5 reanimation treatment logic
	*    Handles all visual effects, equipments and animations for the reanimation process
	*    Started and stopped by the "TreatPersonAction"
	*/
	class EM5_API_EXPORT ReanimationTreatmentLogic : public BaseTreatmentLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReanimationTreatmentLogic" game logic type ID

		enum TREATMENT_STATE
		{
			INIT,
			BASE_TO_MONITOR,
			GET_DEFIBRILLATOR,
			GET_PADS,
			REANIMATION_LOOP,
			REANIMATION_END,
			TURN_BACK_TO_DEFIBRILLATOR,
			PUT_PADS_BACK,
			FINISH_ALL
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ReanimationTreatmentLogic();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ReanimationTreatmentLogic();

		void init(qsf::Entity& doctorEntity, qsf::Entity& targetEntity);
		void stopReanimation();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual methods                             ]
	//[-------------------------------------------------------]
	protected:
		// Job update
		virtual void updateSimulation(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void getDefibrillatorPads();
		bool playReanimationLoop();
		void putPadsBack();

		/*
		*   @brief
		*    Search the doctor equipment for the defibrillator pads and safe them
		*/
		void findDefibrillatorPads();
		void linkEquipmentToTarget(qsf::Entity& equipmentToLink, uint64 targetId, const std::string& boneName, const std::string& childBoneName);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		TREATMENT_STATE				mCurrentState;
		bool						mLoopReanimationAnimation;
		bool						mFirstReanimationLoop;
		qsf::WeakPtr<qsf::Entity>	mDefibrillatorPadRight;
		qsf::WeakPtr<qsf::Entity>	mDefibrillatorPadLeft;

		qsf::WeakPtr<qsf::Entity>	mDefibrillatorPadRightInHand;
		qsf::WeakPtr<qsf::Entity>	mDefibrillatorPadLeftInHand;

		AudioProxy					mDefibrillatorAudioProxy;
		AudioProxy					mEKGAudioProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReanimationTreatmentLogic)
