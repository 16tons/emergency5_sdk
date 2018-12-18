// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/command/mode/debug/DebugCommandMode.h"


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
	*    Trap persons in building or vehicles (there only when the vehicle contains persons) command mode
	*
	*  @remarks
	*    This command mode lets the user select a building entity and trap persons in it. Used for debugging and testing.
	*/
	class EM5_API_EXPORT CloseBuildingDoorCommandMode : public DebugCommandMode
	{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::CloseBuildingDoorCommandMode" unique command pluginable ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		CloseBuildingDoorCommandMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~CloseBuildingDoorCommandMode();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::game::CommandMode methods         ]
		//[-------------------------------------------------------]
	public:
		virtual void startup() override;
		virtual void shutdown() override;
		virtual bool executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds) override;
		virtual void updateCommandMode(const qsf::Clock& clock) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
	private:
		qsf::DebugDrawProxy	mDebugDrawProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::CloseBuildingDoorCommandMode)
