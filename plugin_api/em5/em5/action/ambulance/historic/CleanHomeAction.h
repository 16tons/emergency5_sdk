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
	*    EMERGENCY 2016 clean home action
	*/
	class EM5_API_EXPORT CleanHomeAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::CleanHomeAction" unique action identifier

		enum State
		{
			STATE_INIT,
			STATE_FADEOUT,
			STATE_PROGRESS_INSIDE,
			STATE_STARTCLEANING_ANIMATION,
			STATE_ANIMATION,
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
		CleanHomeAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CleanHomeAction();

		void init(qsf::Entity& buildingEntity);

		uint64 getTargetEntityId() const;


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
		void playDoctorAnimation() const;
		void cleanHome();
		void setMarkXAlpha(float alphaValue);
		void activateMarkX();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<qsf::Entity>	mTargetEntity;
		qsf::WeakPtr<qsf::Entity>	mMarkXDecalEntity;
		State						mState;
		qsf::Time					mProgressTime;
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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CleanHomeAction)