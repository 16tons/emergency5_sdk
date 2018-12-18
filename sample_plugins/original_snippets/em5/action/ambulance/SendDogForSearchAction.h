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
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 send dog for search action
	*/
	class EM5_API_EXPORT SendDogForSearchAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::SendDogForSearchAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_SENDDOG
		};

		enum class UseCase
		{
			RESCUE_DOG_SEARCH_VICTIM,
			POLICE_DOG_SEARCH_OBJECT,
			POLICE_DOG_ATTACK_GANGSTER
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SendDogForSearchAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SendDogForSearchAction();

		void init(qsf::Entity* targetEntity, UseCase useCase, bool* anyFound = nullptr);


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
		void playRescueDogLeaderAnimation() const;
		void pushMoveToTarget();

		void pushMoveToRandomPositions();
		glm::vec3 getRandomPositionInRange(const glm::vec3& entityPosition, const qsf::Map& map, float searchRange);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;
		qsf::WeakPtr<qsf::Entity>	mRescueDogEntity;
		State						mState;
		UseCase						mUseCase;
		bool						mAnyFound;
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
QSF_CAMP_TYPE_NONCOPYABLE(em5::SendDogForSearchAction)
