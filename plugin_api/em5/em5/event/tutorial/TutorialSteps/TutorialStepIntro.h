// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/tutorial/TutorialStep.h"

#include <qsf/job/JobProxy.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/reflection/type/CampQsfTime.h>


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
	*    EMERGENCY 5 tutorial step intro
	*
	*/
	class TutorialStepIntro : public TutorialStep
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TutorialStepIntro(Tutorial& tutorial);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TutorialStepIntro();


	//[-------------------------------------------------------]
	//[ Public virtual em5::TutorialStep methods              ]
	//[-------------------------------------------------------]
	public:
		virtual void startup() override;
		virtual bool update(const qsf::Time& timePassed) override;
		virtual void shutdown() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum class STATE
		{
			STARTUP,
			INTRO_START,
			INTRO_WELCOME,
			INTRO_WAIT_FOR_VIDEO_END,
			INTRO_END,
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onIntroFinished(const qsf::MessageParameters& parameters);
		void onIntroVideoPlaying(const qsf::JobArguments& jobArguments);
		void setupCameraPosition();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		STATE				mCurrentState;
		qsf::Time			mRemainingWaitTimeForIntroSupervisor;
		qsf::Time			mRemainingWaitTimeBeforeStepEnds;
		qsf::MessageProxy	mMessageProxy;							///< Listens to messages that affect the lifecycle
		qsf::JobProxy		mRealtimeUpdateJobProxy;				///< Needed to get updates when the intro video is playing


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
