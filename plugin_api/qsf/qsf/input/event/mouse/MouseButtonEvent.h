// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/event/InputButtonEvent.h"

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
	*    Mouse button event class
	*
	*  @remarks
	*    The mouse button event derives from InputButtonEvent, because it only modifies one input button.
	*    Nevertheless, it can't be recognized as InputButtonEvent via getTypeId(), so it has to generate another
	*    InputButtonEvent in onAddToQueue() to correctly inform all listeners that recognize InputButtonEvents only.
	*/
	class QSF_API_EXPORT MouseButtonEvent : public InputButtonEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 INPUT_EVENT_ID;	///< "qsf::MouseButtonEvent" unique event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MouseButtonEvent(InputButton& button, bool pressed, const glm::vec2& position);

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

		/**
		*  @brief
		*    Apply event to the associated device by changing its state
		*/
		virtual void applyToDevice() const override;


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		glm::vec2 mPosition;	///< The mouse position at the moment the event occurred


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
