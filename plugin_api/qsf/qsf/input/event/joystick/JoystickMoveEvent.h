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
	class JoystickDevice;
	class InputAxis;
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
	*    Joystick move event class
	*
	*  @remarks
	*    In contrast to other the joystick events, this one modifies multiple axes. It derives directly from InputEvent.
	*    Modification of the joystick device is done by more fine-granular JoystickAxisEvent instances created in onAddToQueue().
	*/
	class QSF_API_EXPORT JoystickMoveEvent : public InputEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 INPUT_EVENT_ID;	///< "qsf::JoystickMoveEvent" unique event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		JoystickMoveEvent(JoystickDevice& device, InputAxis& axis, float value);

		/**
		*  @brief
		*    Return associated joystick device
		*/
		JoystickDevice& getJoystickDevice() const;

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
		InputAxis& mAxis;	///< Changed joystick axis
		float	   mValue;	///< The new joystick axis position in interval [-1, 1]


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
