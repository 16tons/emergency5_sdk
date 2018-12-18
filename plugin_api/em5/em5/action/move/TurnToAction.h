// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/action/Action.h>

#include <glm/fwd.hpp>


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
	*    EMERGENCY 5 turn to action
	*/
	class EM5_API_EXPORT TurnToAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::TurnToAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool turnToTarget(qsf::Entity& entity, float timeSeconds, float targetYaw, float turnSpeed = 270.0f);
		static float calcYawRotationToTarget(const qsf::Entity& sourceEntity, const qsf::Entity& targetEntity, bool reverse);


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
		*    "true" if the caller is looking away from the target
		*  @param[in] turnSpeed
		*    Degrees per second the owner is turning to target. 0.0f means turning instant to target.
		*/
		void init(uint64 targetEntityId, bool reverse = false, float turnSpeed = 270.0f);
		void init(const qsf::Entity& targetEntity, bool reverse = false, float turnSpeed = 270.0f);

		void initWithRotation(const glm::quat& rotation, float turnSpeed = 270.0f);
		void initWithRotation(const glm::quat& rotation1, const glm::quat& rotation2, float turnSpeed = 270.0f);

		/**
		*  @brief
		*    Add an alternative rotation, we will pick the nearest rotation when the action starts
		*/
		void addAlternativeRotation(const glm::quat& rotation);


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
		virtual bool onStartup() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64	mTargetEntityId;
		float	mTurnSpeed;
		bool	mReverse;
		float	mTargetYaw;

		std::vector<float>	mAlternativeYaws;	// Multiple Yaws to get the nearest rotations


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::TurnToAction)
