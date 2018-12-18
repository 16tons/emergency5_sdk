// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/reflection/CampClass.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <boost/optional.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Command context base class
		*/
		class QSF_GAME_API_EXPORT CommandContext
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline CommandContext();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CommandContext();

			/**
			*  @brief
			*    Return true if a target position exists
			*/
			inline bool hasTargetPosition() const;

			/**
			*  @brief
			*    Return target position, in case the target position is not valid, glm::vec3(0,0,0) is returned, use hasTargetPosition() to check if target exists
			*/
			const glm::vec3& getTargetPosition() const;


		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			Entity*						mCaller;				///< Caller entity for the command; always valid, do not destroy
			Entity*						mTargetEntity;			///< Target entity for the command; may be a null pointer if no target available, do not destroy
			boost::optional<glm::vec3>	mTargetPosition;		///< Target position if there is one available
			boost::optional<glm::quat>	mTargetRotation;		///< Target rotation if there is one available
			uint64						mUserData;				///< Optional user data to send in "em5::Messages::EM5_SPAWN_UNIT", "qsf::getUninitialized<uint64>()" if there's no user data
			bool						mTargetPositionSealed;	///< Indicates if the given target position is sealed and thus shouldn't be modified
			bool						mAllowSelfExecution;	///< If "true", self-execution commands are allowed (i.e. no target given, or the target is one of the executing callers)
			std::vector<Entity*>		mCallers;				///< The caller entities for which the same command is executed in a row; valid only in command execution
			size_t						mCurrentCallerIndex;	///< The index of the current caller; valid only in command execution


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/command/CommandContext-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE(qsf::game::CommandContext)
