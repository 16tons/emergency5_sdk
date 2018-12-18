// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/input/handler/InputHandler.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class KeyboardEvent;
	class MouseButtonEvent;
	class Entity;
	class EntitySelectionRectangle;
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
	*    EMERGENCY 5 fire simulation input handler class
	*/
	class FireSimulationPreviewInputHandler : public qsf::InputHandler
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit FireSimulationPreviewInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FireSimulationPreviewInputHandler();

		/**
		*  @brief
		*    Update method that gets regularly called
		*/
		void update();


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

		qsf::Entity* pickEntityAtWindowPosition(int positionX, int positionY, glm::vec3* outHitPosition = nullptr) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::EntitySelectionRectangle* mSelectionRectangle;
		bool						   mRightMouseButtonDown;
		glm::vec2					   mRightMouseButtonDragStart;
		bool						   mRightMouseButtonWasDragged;
		qsf::MessageProxy			   mMessageProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
