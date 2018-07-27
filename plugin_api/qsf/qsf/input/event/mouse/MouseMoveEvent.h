// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/event/InputEvent.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class MouseDevice;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Mouse move event class
	*
	*  @remarks
	*    In contract to other the mouse events, this one modifies multiple axes. It derives directly from InputEvent.
	*    Modification of the mouse device is done by more fine-granular MouseAxisEvent instances created in onAddToQueue().
	*/
	class QSF_API_EXPORT MouseMoveEvent : public InputEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 INPUT_EVENT_ID;	///< "qsf::MouseMoveEvent" unique event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MouseMoveEvent(MouseDevice& device, const glm::vec2& position, const glm::vec2& movement);

		/**
		*  @brief
		*    Return associated mouse device
		*/
		MouseDevice& getMouseDevice() const;

		/**
		*  @brief
		*    Return type ID, which is the string hash of the event class name
		*/
		virtual uint32 getTypeId() const override;

		/**
		*  @brief
		*    Called right before event is inserted into the input system's event queue
		*/
		virtual void onAddToQueue() const override;


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		glm::vec2	mPosition;	///< The new mouse position
		glm::vec2	mMovement;	///< Difference to the mouse position set before


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
