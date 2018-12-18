// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/selection/SelectionManager.h"
#include "em5/audio/AudioProxy.h"

#include <qsf/input/handler/InputHandler.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class Component;
	class KeyboardEvent;
	class MouseButtonEvent;
	class MouseMoveEvent;
	class EntitySelectionRectangle;
	namespace game
	{
		class CommandManager;
	}
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
	*    EMERGENCY 5 main game input handler class
	*/
	class EM5_API_EXPORT GameInputHandler : public qsf::InputHandler
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit GameInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GameInputHandler();

		/**
		*  @brief
		*    Returns "true" if the command button with the given index is currently pressed
		*/
		bool isCurrentCommandButtonPressedByIndex(size_t index) const;

		/**
		*  @brief
		*    Reset the internal value for the current command button pressed
		*/
		void resetCurrentCommandButtonPressed();

		/**
		*  @brief
		*    Return whether or not the right mouse button usage currently counts as click or drag
		*/
		bool getRightMouseButtonWasDragged() const;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InputHandler methods           ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateInputHandler(const qsf::Time& timePassed) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InputListener methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void eventReceived(const qsf::InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		qsf::Entity* getEntityForSelection(const qsf::Component& component) const;

		void processKeyboardEvent(const qsf::KeyboardEvent& keyboardEvent);
		void processMouseButtonEvent(const qsf::MouseButtonEvent& mouseButtonEvent);

		void onLeftDown(const qsf::MouseButtonEvent& mouseButtonEvent);
		void onLeftUp(const qsf::MouseButtonEvent& mouseButtonEvent);
		void onLeftClick(const qsf::MouseButtonEvent& mouseButtonEvent);

		void onRightDown(const qsf::MouseButtonEvent& mouseButtonEvent);
		void onRightUp(const qsf::MouseButtonEvent& mouseButtonEvent);
		void onRightClick(const qsf::MouseButtonEvent& mouseButtonEvent);

		void onEscape();
		// Pressed the number [1-9] on non numpad to select or save unit group
		void onNumber(uint32 pressedGroup, bool anyControlPressed);
		// Opens the chat window in multip player game if it is hidden
		void onReturn();

		void selectNextUnit(bool forward, bool jumpToUnit);
		void selectNextUnitSameType(bool forward);
		uint64 getNextUnitInList(const std::vector<uint64>& unitIds, uint64 selectedId, bool forward) const;

		/**
		*  @brief
		*    Listen to selection changes and show them in debug command box
		*
		*  @todo
		*    Currently this function is only for debug reasons
		*/
		void updateDebugCommandBox();

		SelectionManager::IdSet getAllSameUnits(qsf::Entity& entity) const;
		SelectionManager::IdSet getAllSameVisibleUnits(qsf::Entity& entity) const;

		void goToNextFreeplayEvent();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::EntitySelectionRectangle* mSelectionRectangle;
		bool						   mLeftMouseButtonDown;
		bool						   mRightMouseButtonDown;
		glm::vec2					   mRightMouseButtonDragStart;
		bool						   mRightMouseButtonWasDragged;

		// Double click
		glm::vec2		mDoubleClickStart;
		bool			mDoubleClickIsActive;
		qsf::Time		mDoubleClickDuration;

		// Key press
		uint32			mCurrentCommandButtonPressed;	// Watches if a commando shortcut key is pressed and detects the key release event; 0 = no key is pressed

		AudioProxy		mAudioProxy;

		bool			mCurrentInputBlockedState;

		// For cycling through active events
		int	mCurrentEventIndex;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
