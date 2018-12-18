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
	*    EMERGENCY 5 decontaminate person action
	*/
	class EM5_API_EXPORT DecontaminatePersonAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::DecontaminatePersonAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DecontaminatePersonAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DecontaminatePersonAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const qsf::Entity& targetEntity, const qsf::Time& decontaminationTime);

		uint64 getTargetEntityId() const;
		uint64 getContaminatedEntityId() const;


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
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		glm::vec3 findPositionForInjured() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_OPEN_DOOR1,
			STATE_CLOSE_DOOR1,
			STATE_DECONTAMINATION,
			STATE_OPEN_DOOR2,
			STATE_LEAVE_VEHICLE
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64						mTargetEntityId;			///< Target entity ID
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;				///< The firefighter squad entity which interacts with the vehicle
		qsf::WeakPtr<qsf::Entity>	mContaminatedEntity;		///< Contaminated target, is carried by the target (firefighter)
		qsf::Time					mDecontaminationTime;		///< Time how long the decontamination will take
		qsf::Time					mDecontaminationTimeRest;	///< Time how long the decontamination still takes

		// Internal
		State		mCurrentState;	///< Current action progress state
		qsf::Time	mFadeOutTime;
		qsf::Time	mFadeInTime;
		AudioProxy	mAudioProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::DecontaminatePersonAction)
