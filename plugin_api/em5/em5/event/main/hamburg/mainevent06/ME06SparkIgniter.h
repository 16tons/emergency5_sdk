// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
}
namespace qsf
{
	class Entity;
	class ParticlesComponent;
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
	*    ME06 has three objects that cause sparks when burning
	*
	*  @remarks
	*    These sparks can set other objects ablaze after a certain time.
	*    This class is meant to manage the whole stuff.
	*/
	class ME06SparkIgniter
	{
	public:
		ME06SparkIgniter();
		~ME06SparkIgniter();

	public:
		void initialise(FreeplayEvent* freeplayEvent, qsf::Time ignitionTime, qsf::Entity* igniter, qsf::Entity* sparks, qsf::Entity* ignitable);
		void update(const qsf::JobArguments& jobArguments);

	private:
		void onIgnite();
		void onExtinguish();
		void onTargetExtinguished();

	// Proxys
	private:
		qsf::JobProxy		mTimer;
		qsf::MessageProxy	mStartBurningProxy;
		qsf::MessageProxy	mStopBurningProxy;
		qsf::MessageProxy	mTargetStopBurningProxy;

	// Required stuff
	private:
		FreeplayEvent*				mFreeplayEvent;
		uint64						mIgniterId;
		uint64						mIgnitableId;
		qsf::ParticlesComponent*	mSparksParticlesComponent;
		qsf::Entity*				mIgnitableEntity;
		qsf::Time					mSparksFlyingTime;
		qsf::Time					mIgnitionTime;
		bool						mIsBurning;
	};


}
