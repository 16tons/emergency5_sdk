// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/activity/Activity.h"

#include <qsf/map/Entity.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class DebugDrawProxy;
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
	*    Activity player
	*/
	class ActivityPlayer
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ActivityPlayer();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ActivityPlayer();

		bool isPlaying() const;

		void start(qsf::Entity& entity, Activity& activity);
		void stop();

		/**
		*  @brief
		*    Update the activity playback
		*/
		void update(const qsf::Clock& clock);

		void writeDebugOutput(qsf::DebugDrawProxy& debugDrawProxy);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool switchToState(uint32 stateId);
		bool switchToStep(uint32 stepIndex);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Configuration
		qsf::WeakPtr<qsf::Entity> mEntity;
		Activity*				  mActivity;

		// Activity player state
		bool					  mIsPlaying;
		uint32					  mActivityLoadVersion;
		uint32					  mCurrentStateId;
		const Activity::State*	  mCurrentState;
		uint32					  mCurrentStepIndex;
		ActivityStep*			  mCurrentStep;
		qsf::Time				  mCurrentStepTime;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
