// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/command/Command.h"
#include "em5/ai/MovementModes.h"


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
	*    EMERGENCY 5 move command implementation
	*/
	class MoveCommand : public Command
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::MoveCommand" unique command pluginable ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		static void computeMultiSelectionTargetPositionForOrdering(qsf::Entity& caller, int index_, int numOrder_, glm::vec3& position);
		static bool computeAvailableTargetPosition(qsf::Entity& caller, glm::vec3& position);
		static bool computeAvailableTargetPosition(qsf::Entity& caller, glm::vec3& position, const float searchRadius);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit MoveCommand(qsf::game::CommandManager* commandManager);


	//[-------------------------------------------------------]
	//[ Public virtual em5::Command methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual bool checkAvailable() override;
		virtual bool checkCaller(qsf::Entity& caller) override;
		virtual bool checkContext(const qsf::game::CommandContext& context) override;
		virtual void execute(const qsf::game::CommandContext& context) override;
		virtual bool highlightTarget() const override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		MovementModes::Modes getMovementMode(qsf::Entity& entity) const;

		static void computeMultiSelectionTargetPosition(qsf::Entity& caller, const std::vector<qsf::Entity*> callers, glm::vec3& position);
		static bool isTargetPositionBlockedByPolygons(qsf::Entity& caller, const glm::vec3& position);


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MoveCommand)