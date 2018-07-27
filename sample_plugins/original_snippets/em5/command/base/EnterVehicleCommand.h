// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/command/Command.h"


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
	*    EMERGENCY 5 enter vehicle command
	*
	*  @note
	*    - The Emergenyc 5 doctor logic manipulates this command
	*/
	class EnterVehicleCommand : public Command
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::EnterVehicleCommand" unique command pluginable ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit EnterVehicleCommand(qsf::game::CommandManager* commandManager);

		/**
		*  @brief
		*    Perform the caller check, but skips the priority check. Useful for the check if an action, pushed by logic/automatism has an valid caller
		*/
		bool checkCallerWithoutPriority(qsf::Entity& caller);


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual bool checkAvailable() override;
		virtual bool checkCaller(qsf::Entity& caller) override;
		virtual bool checkContext(const qsf::game::CommandContext& context) override;
		virtual void execute(const qsf::game::CommandContext& context) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Push all actions necessary for this commando
		*/
		void pushEnterVehicleActions(qsf::ActionComponent& actionComponent, const qsf::game::CommandContext& context);
		void pushEnterBoatActions(qsf::ActionComponent& actionComponent, const qsf::game::CommandContext& context);
		void pushEnterTSEKRActions(qsf::ActionComponent& actionComponent, const qsf::game::CommandContext& context);

		bool isDiverAndBoat(const qsf::game::CommandContext& context);
		bool isSEKRobotAndTransporter(const qsf::game::CommandContext& context);
		bool isParamedicAndRescueHeli(const qsf::game::CommandContext& context);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 ACTION_PRIORITY;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::EnterVehicleCommand)
