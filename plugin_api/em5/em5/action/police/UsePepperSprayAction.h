// Copyright (C) 2012-2017 Promotion Software GmbH


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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 use pepper spray action
	*/
	class EM5_API_EXPORT UsePepperSprayAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::UsePepperSprayAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_BRING_SPRAY_IN_POSITION,
			STATE_STARTUSING_PEPPERSPRAY,
			STATE_USEPEPPERSPRAY,
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
		UsePepperSprayAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UsePepperSprayAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity);

		/**
		*  @brief
		*    Give back entity id of our target
		*/
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
		virtual void onShutdown() override;
		virtual void onPause() override;
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity* createPepperSprayEffect();
		void linkEffectEntity();
		bool validateUserAndTarget() const;
		void setTargetStunned();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64						mTargetEntityId;			///< Target entity of the action
		State						mCurrentState;				///< Current action progress state
		qsf::Time					mTimePepperSprayIsInUse;	///< Time the pepper spray is used by the entity
		qsf::Time					mTimeAnimationBlending;		///< Time the policeman need to aim with the pepperspray, this is also the logical animation blending time
		qsf::WeakPtr<qsf::Entity>	mEffectEntity;				///< Entity with the particle spray effect
		float						mTargetYaw;
		AudioProxy					mAudioProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::UsePepperSprayAction)
