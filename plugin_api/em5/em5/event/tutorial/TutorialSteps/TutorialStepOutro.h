// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/event/tutorial/TutorialStep.h"

#include <qsf/map/Entity.h>
#include <qsf/message/MessageProxy.h>
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
	*    EMERGENCY 5 tutorial step outro
	*
	*/
	class TutorialStepOutro : public TutorialStep
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TutorialStepOutro(Tutorial& tutorial);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TutorialStepOutro();


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
			INIT,
			STARTUP,
			OUTRO_START,
			WAIT_FOR_SUPERVISOR38_END,
			WAIT_FOR_STW_REACHES_HQ,
			SETUP_FADE_TO_BLACK,
			WAIT_FOR_FADE_TO_BLACK,
			OUTRO_END,
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onReturnToBaseExecuted(const qsf::MessageParameters& parameters);
		void onFadeoutFinished(const qsf::MessageParameters& parameters);
		// TODO(sw) Only needed for gamescom version
		void setupMoveCameraToHq();
		void setupForOutroCutscene();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		STATE				mCurrentState;
		qsf::WeakPtr<qsf::Entity> mFollowEntity;
		qsf::Time			mRemainingWaitTimeBeforeSupervisor38Start;
		qsf::MessageProxy	mMessageProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
