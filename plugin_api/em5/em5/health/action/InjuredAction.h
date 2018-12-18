// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/health/HealthComponent.h"

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
	*    EMERGENCY 5 injured action
	*
	*  @remarks
	*    Takes care of all injury animation and blocks other actions.
	*    This action is pushed with blocking priority and is never ending.
	*/
	class EM5_API_EXPORT InjuredAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::InjuredAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		InjuredAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~InjuredAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(HealthComponent::State state, bool playCollapseAnimation, bool disableAnimationPreventing = false);

		bool getPlayCollapseAnimation() const;


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
		void switchToInjuryState();
		void switchToDeadState();
		bool switchToDeadAnimation();

		void resetAudioTimer();
		void playInjuryAudio();
		std::string buildAudioFilePath();

		void autoUpdateBoundingBox();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HealthComponent::State			mLastState;					///< State of the health component in the last update
		bool							mPlayCollapseAnimation;		/// play the collapse animation when true and it makes sense for the entity
		qsf::Time						mTimeTillNextAudio;			///< The time till the next audio is starting. Audio is looped in an random interval.
		qsf::WeakPtr<HealthComponent>	mHealthComponent;			///< Pointer to the health component of the target
		int								mNumberNonCoughingAudio;	///< The number of non coughing audio till next counghting audio is coming [3 means 3 times normal audio, then coughing audio]
		bool							mCanPushKneeToLyingAnimation;
		bool							mCanDisableAnimationPreventing;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::InjuredAction)
