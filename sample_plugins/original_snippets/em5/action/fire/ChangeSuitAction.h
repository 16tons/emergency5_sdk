// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>
#include <qsf/asset/AssetProxy.h>


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
	*    EMERGENCY 5 change suit action. Currently used for fireman switching default and ABC suit.
	*/
	class EM5_API_EXPORT ChangeSuitAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::ChangeSuitAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ChangeSuitAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ChangeSuitAction();

		/**
		*  @brief
		*    Initialize action with values; fadeOutTime must be at least as long as the animation to change suits
		*/
		void init(const qsf::AssetProxy& targetSuit, const qsf::Time& fadeOutTime, const qsf::Time& fadeInTime);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_FADE_OUT_EQUIPMENT,
			STATE_FADE_OUT_START,
			STATE_FADE_OUT,
			STATE_FADE_IN,
			STATE_FADE_IN_EQUIPMENT
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State			mState;
		qsf::AssetProxy	mTargetSuit;
		qsf::Time		mFadeEquipmentTime;
		qsf::Time		mFadeOutTime;
		qsf::Time		mFadeInTime;
		qsf::Time		mTimeUntilStateSwitch;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ChangeSuitAction)
