// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/event/InputAxisEvent.h"


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
	*    Mouse wheel event class
	*
	*  @remarks
	*    The mouse wheel event derives from InputAxisEvent, because it only modifies one input axis.
	*    Nevertheless, it can't be recognized as InputAxisEvent via getTypeId(), so it has to generate another
	*    InputAxisEvent in onAddToQueue() to correctly inform all listeners that recognize InputAxisEvents only.
	*/
	class QSF_API_EXPORT MouseWheelEvent : public InputAxisEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 INPUT_EVENT_ID;	///< "qsf::MouseWheelEvent" unique event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MouseWheelEvent(MouseDevice& device, float movement);

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


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
