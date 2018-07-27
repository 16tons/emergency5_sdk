// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/command/evaluation/CommandEvaluationHelper.h> // For qsf::game::CommandEvaluationHelper::CommandInfo. Forward declaration for nested classes is not possible
#include <qsf_game/command/mode/CommandMode.h>

#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class CommandableComponent;
	struct RayMapQueryResponse;
}


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
	*    Default command mode for EMERGENCY 5
	*/
	class EM5_API_EXPORT DefaultCommandMode : public qsf::game::CommandMode
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::DefaultCommandMode" unique command mode pluginable ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Shows a visual feedback
		*/
		static void showVisualCommandFeedback(const qsf::game::CommandContext& context);

		/**
		*  @brief
		*    Returns if the unit is allowed for selection
		*
		*  @param[in]  commandableComponent
		*    The commandable component of the unit to check
		*
		*  @note
		*    - A unit is allowed for selection, if the unit belongs to the current player (when this option is active) or the unit is injured/destroyed
		*/
		static bool isUnitAllowedForSelection(const CommandableComponent& commandableComponent);

		/**
		*  @brief
		*    Returns if the unit is allowed for selection
		*
		*  @param[in] entity
		*    The entity to check
		*
		*  @return
		*     "true" when the entity is a unit and belongs to the current player
		*/
		static bool isUnitAllowedForSelection(const qsf::Entity& entity);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		DefaultCommandMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DefaultCommandMode();

		qsf::Time getWaitTimeBetweenChecks() const;
		void setWaitTimeBetweenChecks(const qsf::Time& waitTimeBetweenChecks);
		void onRayMapQueryResponse(const RayMapQueryResponse& rayMapQueryResponse);

		/**
		*  @brief
		*    Return the ID of the currently highlighted entity
		*/
		uint64 getMouseoverHighlightedEntityId() const;

		/**
		*  @brief
		*    Sets the entity instance for which the command highlighting should only be done
		*
		*  @param[in] commandHighlightOnlyThisEntity
		*    The entity for which only command highlighting should be done, can be a nullptr to disable this restriction
		*/
		void setOnlyCommandHighlightingEntity(qsf::Entity* commandHighlightOnlyThisEntity);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	public:
		virtual void startup() override;
		virtual void shutdown() override;
		virtual bool executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds) override;
		virtual void updateCommandMode(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum MouseCursorIcon
		{
			MOUSECURSORICON_DEFAULT,
			MOUSECURSORICON_BLOCKED,
			MOUSECURSORICON_FROM_COMMAND
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void highlightEntityUnderMouse(qsf::Entity* entityUnderMouse);

		/**
		*  @brief
		*    Shows which command will be executed on an "execute command" request for the entity which is under the mouse cursor
		*	 The mouse cursor image will be changed (TODO(sw) currently only a text is shown beneath the cursor)
		*/
		void showCommandWhichCanBeExecuted(qsf::Entity* entityUnderMouse, const glm::vec3& hitPosition);
		void updateDisplayOfExecutableCommand();

		/**
		*  @brief
		*    Set default mouse cursor
		*/
		void resetToDefault();

		/**
		*  @brief
		*    Set blocked mouse cursor
		*/
		void setBlockedMouseCursor();

		void resetCommandTargetHighlighting();
		void resetSelectableHighlighting();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time															mWaitTimeBetweenChecks;	///< With this wait time we become a small but noticeable delay when the entity under the mouse changes
		uint64																mMouseoverHighlightedEntityId;
		uint64																mCommandTargetHighlightedEntityId;
		qsf::Time															mElapsedWaitTime;
		boost::optional<qsf::game::CommandEvaluationHelper::CommandInfo>	mLastCommandInfo;
		qsf::Entity*														mLastEntityUnderMouse;
		qsf::Entity*														mEntityForCommandHighlighting;
		MouseCursorIcon														mCurrentMouseCursorIcon;

		// Debug
		qsf::DebugDrawProxy	mDebugDrawProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
