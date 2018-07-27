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
	*    EMERGENCY 5 rope down SEK from (flying) helicopter action
	*/
	class EM5_API_EXPORT RopeDownSEKFromHelicopterAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::RopeDownSEKFromHelicopterAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_LOWER_ROPE,
			STATE_LOWER_UNIT,
			STATE_FINISH
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		RopeDownSEKFromHelicopterAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~RopeDownSEKFromHelicopterAction();

		void init(qsf::Entity& parentEntity, qsf::Entity& fastRopeEntity, int ropePlace);


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
		void createRope();
		void updateAnimation(const qsf::JobArguments& jobArgs);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State						mState;
		float						mFalldownSpeed;		///< The speed in m/s the person is coming near to the ground. There is an small random amount to the value for everyone.
		float						mGroundHeight;
		qsf::WeakPtr<qsf::Entity>	mVehicleEntity;		///< The entity were the owner was (or better is currently) inside
		qsf::WeakPtr<qsf::Entity>	mFastRopeEntity;	///< The entity were we spawn the rope
		qsf::WeakPtr<qsf::Entity>	mRopeEntity;		///< Created and linked to the fastRope entity. We destroy it after the action.
		int							mRopePlace;			///< The number of place in the rope (1-3)
		bool						mIsNearGround;		///< "true" if the person is below x meter from the ground. Then trigger animation.
		AudioProxy					mAudioProxy;

		// Smooth animation
		qsf::JobProxy				mUpdateAnimation;
		State						mAnimationState;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::RopeDownSEKFromHelicopterAction)
