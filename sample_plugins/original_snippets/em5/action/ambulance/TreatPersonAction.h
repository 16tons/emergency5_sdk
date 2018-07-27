// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/logic/action/Action.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class HealthComponent;
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
	*    EMERGENCY 5 treat person action
	*/
	class EM5_API_EXPORT TreatPersonAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::TreatPersonAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_GET_DOWN,
			STATE_OPEN_MEDIKIT,
			STATE_DIAGNOSIS,
			STATE_RUN_DIAGNOSIS,
			STATE_HEAL,
			STATE_REANIMATE_START,
			STATE_REANIMATE,
			STATE_CLOSE_MEDIKIT,
			STATE_GET_UP
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TreatPersonAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TreatPersonAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(qsf::Entity& targetEntity);

		uint64 getTargetEntityId() const;

		uint64 getMedikitId() const;


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
		glm::vec3 calculateOptimalDoctorPosition(bool originalSide = true) const;
		void checkAndMoveDoctorToOptimalPosition();
		void checkAndMoveTargetToOptimalPosition() const;
		void placeMedikit();
		void takeMedikit();
		void openMedikit() const;
		void closeMedikit() const;
		bool checkReanimationState() const;
		void bringTargetInPosition() const;
		void startDoctorDiagnosis() const;
		void stopDoctorDiagnosis() const;
		void startDoctorMedicalCare() const;
		bool isDoctorMedicalCareFinished() const;
		void stopDoctorMedicalCare() const;
		void startReanimationTreatment() const;
		void stopReanimationTreatment() const;
		bool isTreatmentLogicActive() const;
		void setGloves(bool hasGloves) const;
		void deleteTreatmentLogic() const;
		void showDebug();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State						  mState;
		bool						  mMedikitPlaced;
		qsf::WeakPtr<qsf::Entity>	  mMedikitEntity;
		qsf::WeakPtr<qsf::Entity>	  mTargetEntity;
		qsf::WeakPtr<HealthComponent> mTargetHealthComponent;
		glm::vec3					  mOptimalDoctorPosition;
		bool						  mDoctorIsMovedToMirroredTargetSide;

		// Debug
		qsf::DebugDrawProxy mDebugDrawProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::TreatPersonAction)
