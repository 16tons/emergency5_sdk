// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    EMERGENCY 5 exit vehicle command
	*
	*  @note
	*    - The EMERGENCY 5 doctor logic manipulates this command
	*/
	class ExitVehicleCommand : public Command
	{


	//[-------------------------------------------------------]
	//[ Static public methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Collect entities that should usually be unloaded (e.g. no injureds, arrested gangsters)
		*/
		static void getEntitiesToExit(qsf::Entity& callerEntity, std::vector<qsf::Entity*>& entitiesToExit);

		/**
		*  @brief
		*    Check for a certain entity to be unloaded (if parameter is initialized, otherwise this does the same as "getEntitiesToExit")
		*/
		static void getEntitiesToExitWithFilter(qsf::Entity& callerEntity, std::vector<qsf::Entity*>& entitiesToExit, uint64 filterId);

		/**
		*  @brief
		*    Collect entities to unloaded, without the usual filtering, like exclusion of injureds
		*/
		static void getEntitiesToExitUnfiltered(qsf::Entity& callerEntity, std::vector<qsf::Entity*>& entitiesToExit);


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::ExitVehicleCommand" unique command pluginable ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit ExitVehicleCommand(qsf::game::CommandManager* commandManager);


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual bool checkAvailable() override;
		virtual bool checkCaller(qsf::Entity& caller) override;
		virtual bool checkContext(const qsf::game::CommandContext& context) override;
		virtual void execute(const qsf::game::CommandContext& context) override;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::ExitVehicleCommand)
