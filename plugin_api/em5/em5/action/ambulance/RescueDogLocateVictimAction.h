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
	*    EMERGENCY 5 rescue dog locate victim action
	*/
	class EM5_API_EXPORT RescueDogLocateVictimAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::RescueDogLocateVictim" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_PANT,
			STATE_FOUND_VICTIM,
			STATE_LOOPACTION
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RescueDogLocateVictimAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RescueDogLocateVictimAction();

		void init(qsf::Entity& targetEntity, qsf::Entity& rescueDogLeaderEntity);


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
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor for sub-classes
		*/
		RescueDogLocateVictimAction(uint32 actionId);


	//[-------------------------------------------------------]
	//[ Protected virtual em5::RescueDogLocateVictimAction methods ]
	//[-------------------------------------------------------]
	protected:
		virtual uint32 getFoundMessageId() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void dogReturnToLeader() const;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;
		qsf::WeakPtr<qsf::Entity>	mRescueDogLeaderEntity;
		bool						mTurnToTarget;
		qsf::Time					mTimeForAutoAbort;
		qsf::WeakPtr<qsf::MeshAnimationChannel>	mMeshAnimationChannel;

		State						mState;
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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RescueDogLocateVictimAction)
