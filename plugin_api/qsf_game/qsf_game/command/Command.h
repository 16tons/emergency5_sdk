// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/command/CommandManager.h"

#include <qsf/base/NamedIdentifier.h>
#include <qsf/plugin/pluginable/Pluginable.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class ActionComponent;
	namespace game
	{
		class CommandContext;
	}
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
		*    Abstract command base class
		*/
		// TODO(sw) add a method which returns a (mouse cursor) image which represents the command visually
		class QSF_GAME_API_EXPORT Command : public Pluginable<CommandManager>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			Command(CommandManager* commandManager, uint32 commandId);

			/**
			*  @brief
			*    Get command ID, which is the command class's PLUGINABLE_ID
			*/
			inline uint32 getId() const;

			/**
			*  @brief
			*    Get command priority
			*/
			inline uint32 getPriority() const;

			/**
			*  @brief
			*    Returns true if command is created via script
			*/
			inline bool getIsScriptCommand() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::Command methods             ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Check whether the command is currently available at all
			*
			*  @return
			*    "true" if command can be executed, "false" if it is blocked or makes no sense in the current game state
			*/
			virtual bool checkAvailable() = 0;

			/**
			*  @brief
			*    Check whether the command can be executed by the given caller entity
			*
			*  @param[in] caller
			*    The caller entity to check
			*
			*  @return
			*    "true" if command can be executed by this entity, "false" if the entity is not able to execute the command (at all or in the current situation)
			*
			*  @remarks
			*    This only gets called in case checkAvailable() returned "true", so don't repeat checks from checkAvailable() in checkCaller().
			*/
			virtual bool checkCaller(Entity& caller) = 0;

			/**
			*  @brief
			*    Check whether the command can be executed with the given context
			*
			*  @param[in] context
			*    The command context, including caller and target information
			*
			*  @return
			*    "true" if command can be executed for this context, "false" if the target is not valid for the command
			*
			*  @remarks
			*    This only gets called if both checkAvailable() and checkCaller() returned "true", so don't repeat their checks in checkContext().
			*/
			virtual bool checkContext(const CommandContext& context) = 0;

			/**
			*  @brief
			*    Execute command for given context
			*
			*  @param[in] context
			*    The command context, including caller and target information
			*
			*  @remarks
			*    This only gets called if all three checks returned "true".
			*    Command execution usually consists of pushing new actions to the caller entity's action component.
			*/
			virtual void execute(const CommandContext& context) = 0;

			/**
			*  @brief
			*    Returns the name of the command
			*/
			virtual std::string getName() const;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			bool checkCallerActionPriority(Entity& caller, uint32 maximumPriority) const;

			ActionComponent& getActionComponent(Entity& caller) const;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			uint32 mPriority;			///< Commands with higher priority get evaluated first; default value is 100
			bool   mIsScriptCommand;	///< True if command is loaded via script


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32	mCommandId;		///< ID of the concrete command instance; should always be the command class's PLUGINABLE_ID


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/command/Command-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::Command)
