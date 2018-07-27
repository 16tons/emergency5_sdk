// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_game/command/CommandDelegate.h>

#include <boost/container/flat_set.hpp>


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
	*    EMERGENCY 5 command delegate instance
	*/
	class CommandDelegate : public qsf::game::CommandDelegate
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		CommandDelegate();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~CommandDelegate();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandDelegate methods     ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool areCommandsAllowed() override;
		inline virtual bool isCommandAllowed(const qsf::game::Command& command) override;
		virtual bool isCallerAllowed(qsf::Entity& caller, const qsf::game::CommandContext& context) override;
		virtual bool isTargetAllowed(const qsf::game::CommandContext& context) override;
		virtual bool preCommandExecution(const qsf::game::Command& command, const std::vector<qsf::Entity*>& callers, qsf::game::CommandContext& context) override;
		virtual bool preCommandExecution(const qsf::game::Command& command, qsf::game::CommandContext& context) override;
		virtual void postCommandExecution(const qsf::game::Command& command, const qsf::game::CommandContext& context) override;
		virtual qsf::game::CommandMode* getDefaultCommandMode() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void playCommandResponse(const qsf::game::Command& command, const qsf::game::CommandContext& context);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::container::flat_set<uint32> mCommandsWhichActivatesCommandModes;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/command/CommandDelegate-inl.h"
