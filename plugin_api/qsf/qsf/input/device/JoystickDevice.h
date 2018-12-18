// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/device/InputDevice.h"
#include "qsf/input/control/InputButton.h"
#include "qsf/input/control/InputAxis.h"

#include <glm/glm.hpp>


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
	*    Joystick input device class
	*/
	class QSF_API_EXPORT JoystickDevice : public InputDevice
	{


	//[-------------------------------------------------------]
	//[ Device definition                                     ]
	//[-------------------------------------------------------]
	public:
		// Axes
		InputAxis	XAxis;			///< X axis absolute position
		InputAxis	YAxis;			///< Y axis absolute position
		InputAxis	ZAxis;			///< Z axis absolute position - for modern gamepads, this is the right analog stick's X axis
		InputAxis	RAxis;			///< R axis absolute position - for modern gamepads, this is the right analog stick's Y axis (also "Z rotation" axis)
		InputAxis	LeftTrigger;	///< Left shoulder trigger absolute position (e.g. XBox controller); range is 0.0f (not pressed) to 1.0f (fully pressed)
		InputAxis	RightTrigger;	///< Right shoulder trigger absolute position (e.g. XBox controller); range is 0.0f (not pressed) to 1.0f (fully pressed)

		// Buttons
		InputButton	Button1;		///< Joystick button #1
		InputButton	Button2;		///< Joystick button #2
		InputButton	Button3;		///< Joystick button #3
		InputButton	Button4;		///< Joystick button #4
		InputButton	Button5;		///< Joystick button #5
		InputButton	Button6;		///< Joystick button #6
		InputButton	Button7;		///< Joystick button #7
		InputButton	Button8;		///< Joystick button #8
		InputButton	Button9;		///< Joystick button #9
		InputButton	Button10;		///< Joystick button #10
		InputButton	Button11;		///< Joystick button #11
		InputButton	Button12;		///< Joystick button #12

		// TODO(fw): Add POV hats, and maybe all the other stuff OIS offers (sliders, vector3d positions)


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		JoystickDevice();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~JoystickDevice();

		/**
		*  @brief
		*    Return current joystick primary stick position as glm::vec2, normalized in [-1, 1] interval
		*
		*  @remarks
		*    For gamepads, this is the position of the left analog stick
		*/
		glm::vec2 getPosition() const;

		/**
		*  @brief
		*    Return current joystick secondary stick position as glm::vec2, normalized in [-1, 1] interval
		*
		*  @remarks
		*    For gamepads, this is the position of the right analog stick
		*/
		glm::vec2 getSecondaryPosition() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputDevice methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void updateDevice();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
