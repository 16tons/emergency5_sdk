// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/action/Action.h"
#include "qsf/reflection/type/CampGlmQuat.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Turn to action
	*/
	class QSF_API_EXPORT TurnToAction : public Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const NamedIdentifier ACTION_ID;	///< "qsf::TurnToAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		TurnToAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~TurnToAction();

		/**
		*  @brief
		*    Initialize action with values
		*
		*  @param[in] targetEntityId
		*    The id of the target the caller turns to
		*  @param[in] reverse
		*    "true" if the caller is looked away from the target
		*  @param[in] turnSpeed
		*    Degrees per second the owner is turning to target. 0.0f means turning instant to target.
		*/
		void init(uint64 targetEntityId, bool reverse = false, float turnSpeed = 270.0f);

		/**
		*  @brief
		*    Initialize action with values
		*
		*  @param[in] rotation
		*    The target rotation which the entity should rotate to
		*  @param[in] turnSpeed
		*    Degrees per second the owner is turning to target. 0.0f means turning instant to target.
		*/
		void initWithRotation(glm::quat rotation, float turnSpeed = 270.0f);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual action::Result updateAction(const Clock& clock) override;
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void calcFinalRotationViaTargetEntity();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64	mTargetEntityId;
		float	mTurnSpeed;
		bool	mReverse;
		float	mTargetYaw;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::TurnToAction)
