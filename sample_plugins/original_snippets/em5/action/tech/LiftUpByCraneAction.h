// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

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
	*    EMERGENCY 5 lift up objects by heavy crane action
	*/
	class EM5_API_EXPORT LiftUpByCraneAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::LiftUpByCraneAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_SUPPORTLEGS_EXPAND,
			STATE_CRANE_ALIGN_ARM,
			STATE_CRANE_MOVE_DOWN_HOOK,
			STATE_CRANE_MOVE_SHAKE_UP_HOOK,
			STATE_CRANE_MOVE_SHAKE_DOWN_HOOK,
			STATE_CRANE_MOVE_UP_HOOK_AND_ENTITY,
			STATE_CRANE_ALIGN_TO_DISPOSE,
			STATE_CRANE_MOVE_DOWN_HOOK_AND_ENTITY,
			STATE_CRANE_MOVE_SHAKE_2_UP_HOOK,
			STATE_CRANE_MOVE_SHAKE_2_DOWN_HOOK,
			STATE_CRANE_MOVE_UP_HOOK,
			STATE_CRANE_FOLD,
			STATE_SUPPORTLEGS_HIDE,
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
		LiftUpByCraneAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~LiftUpByCraneAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
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
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void unhideBuriedEntities(qsf::Entity& targetEntity);
		void finishTargetEntity(qsf::Entity& targetEntity);
		void deleteBuoy(qsf::Entity& targetEntity);
		bool findDisposePosition(qsf::Entity& targetEntity, uint32 numberOfChecks);
		void linkParticles(qsf::Entity& targetEntity, qsf::Entity* particleEntity, float delay, float timeout);
		void unlinkParticles(qsf::Entity& targetEntity);
		bool canReachNextTargetFromSamePosition();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		uint64			mTargetEntityId;
		State			mState;
		glm::vec3		mTargetPosition;
		glm::vec3		mDisposePosition;
		float			mObjectHeight;
		float			mCraneHeight;
		bool			mInWater;
		bool			mTargetPlaced;

		// Only for temporary entities, no weak pointer necessary
		qsf::Entity*	mWatersprayEntity;
		qsf::Entity*	mWaterdropsEntity;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::LiftUpByCraneAction)
