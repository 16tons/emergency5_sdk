// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/System.h>
#include <qsf/reflection/CampClass.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{
		class Command;
		class CommandManager;
		class CommandMode;
		class CommandDelegate;
		class CommandEvaluation;
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
		*    Command system
		*/
		class QSF_GAME_API_EXPORT CommandSystem : public System
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_set<uint64> EntityIdSet;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline CommandSystem();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~CommandSystem();

			/**
			*  @brief
			*    Return the command manager instance (always valid, do not destroy the instance)
			*/
			inline CommandManager& getCommandManager() const;

			/**
			*  @brief
			*    Return the currently set command mode instance (always valid, do not destroy the instance)
			*/
			inline CommandMode& getCommandMode() const;

			/**
			*  @brief
			*    Set the command mode, replacing the existing one; takes over the memory ownership
			*/
			void setCommandMode(CommandMode& commandMode);

			/**
			*  @brief
			*    Set the command mode back to the default mode, replacing the existing one
			*/
			bool resetCommandMode();

			/**
			*  @brief
			*    Set the command mode to one of the given class, replacing the existing one
			*/
			template <typename T>
			void switchToCommandMode();

			/**
			*  @brief
			*    Return the currently set command delegate instance (always valid, do not destroy the instance)
			*/
			inline CommandDelegate& getCommandDelegate() const;

			/**
			*  @brief
			*    Set the command delegate, replacing the existing one
			*/
			void setCommandDelegate(CommandDelegate& commandDelegate);

			/**
			*  @brief
			*    Switch the command delegate to one of the given class, replacing the existing one
			*/
			template <typename T>
			void createCommandDelegate();

			/**
			*  @brief
			*    Switch the command delegate back to the default one
			*/
			void resetCommandDelegate();

			/**
			*  @brief
			*    Return the command evaluation instance (always valid, do not destroy the instance)
			*/
			inline CommandEvaluation& getCommandEvaluation() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		public:
			inline virtual const char* getName() const override;

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup(bool serverMode) override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void setDefaultCommandMode(CommandMode& defaultCommandMode);
			void updateJob(const JobArguments& jobArguments);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			CommandManager*		mCommandManager;		///< Command manager instance, manages the set of all known commands; valid between startup and shutdown
			CommandMode*		mCommandMode;			///< Command mode instance; valid between startup and shutdown
			CommandMode*		mDefaultCommandMode;	///< Default command mode instance; valid between startup and shutdown
			CommandDelegate*	mCommandDelegate;		///< Command delegate instance, application-specific callback responder; valid between startup and shutdown
			CommandEvaluation*	mCommandEvaluation;		///< Command evaluation instance, helper for command execution; valid between startup and shutdown

			JobProxy			mUpdateJobProxy;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/command/CommandSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::game::CommandSystem)
