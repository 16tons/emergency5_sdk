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
	*    EMERGENCY 5 pickup person by paramedic action
	*/
	class EM5_API_EXPORT PickupPersonByParamedicAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::PickupPersonByParamedicAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_LOWER_STRETCHER,
			STATE_DELAY1,
			STATE_LINK_PATIENT,
			STATE_RAISE_STRETCHER,
			STATE_END
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		PickupPersonByParamedicAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PickupPersonByParamedicAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(qsf::Entity& targetEntity);

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
		void findIdFromChildLinks();
		void startLowerStrechAnimation(qsf::Entity& strecherEntity);
		void startLowerParamedicsAnimation(qsf::Entity& frontParamedic, qsf::Entity& backParamedic);
		void startRaiseStrechAnimation(qsf::Entity& strecherEntity);
		void startRaiseParamedicsAnimation(qsf::Entity& frontParamedic, qsf::Entity& backParamedic);
		void startVictimAnimation(qsf::Entity& patientEntity) const;
		void pushWaitAction(qsf::Entity& entity, const qsf::Time& waitTime) const;
		bool isAnimationPlaying() const;
		void sendMessage() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64		mTargetEntityId;
		bool		mKeepTargetReservation;
		State		mState;
		uint64		mStrecherId;
		uint64		mFrontParamedicId;
		uint64		mBackParamedicId;

		AudioProxy	mAudioProxy;

		qsf::WeakPtr<qsf::MeshAnimationChannel>	mStrecherAnimation;
		qsf::WeakPtr<qsf::MeshAnimationChannel>	mFrontParamedicAnimation;
		qsf::WeakPtr<qsf::MeshAnimationChannel>	mBackParamedicAnimation;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::PickupPersonByParamedicAction)
