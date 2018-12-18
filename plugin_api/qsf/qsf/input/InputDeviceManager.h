// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class InputDevice;
	class KeyboardDevice;
	class MouseDevice;
	class JoystickDevice;
	class InputDeviceFeeder;
	class UserInputDeviceFeeder;
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
	*    Input device manager
	*
	*  @remarks
	*    Manages not only the set of all input devices, but also the input device feeders that fill the devices with input data.
	*    There are default devices always present (keyboard and mouse) and a user input device feeder that forwards input data
	*    to user-defined devices through the connections registered there.
	*/
	class QSF_API_EXPORT InputDeviceManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		InputDeviceManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~InputDeviceManager();

		/**
		*  @brief
		*    Update input devices and device feeders
		*/
		void updateDevices();

		//[-------------------------------------------------------]
		//[ Input devices                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Add an input device
		*
		*  @param[in] inputDevice
		*    Input device to add, must stay valid as long as it's registered within the manager
		*
		*  @return
		*    "true" if all went fine, else "false" (input device already registered?)
		*/
		bool addInputDevice(InputDevice& inputDevice);

		/**
		*  @brief
		*    Remove an input device
		*
		*  @param[in] inputDevice
		*    Input device to remove
		*
		*  @return
		*    "true" if all went fine, else "false" (input device not registered?)
		*/
		bool removeInputDevice(InputDevice& inputDevice);

		/**
		*  @brief
		*    Return the keyboard device
		*/
		inline KeyboardDevice& getKeyboard() const;

		/**
		*  @brief
		*    Return the mouse device
		*/
		inline MouseDevice& getMouse() const;

		/**
		*  @brief
		*    Return the joystick device
		*/
		inline JoystickDevice& getJoystick() const;

		//[-------------------------------------------------------]
		//[ Device feeders                                        ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Add an input device feeder
		*
		*  @param[in] deviceFeeder
		*    Input device feeder to add; ownership is taken over by the manager, i.e. it will automatically destroy the feeder
		*
		*  @return
		*    "true" if all went fine, else "false" (input device feeder already registered?)
		*/
		bool addInputDeviceFeeder(InputDeviceFeeder& deviceFeeder);

		/**
		*  @brief
		*    Remove an input device feeder
		*
		*  @param[in] deviceFeeder
		*    Input device feeder to remove
		*
		*  @return
		*    "true" if all went fine, else "false" (input device feeder not registered?)
		*/
		bool removeInputDeviceFeeder(InputDeviceFeeder& deviceFeeder);

		/**
		*  @brief
		*    Return the default feeder for user-defined input devices
		*/
		inline UserInputDeviceFeeder& getUserInputDeviceFeeder() const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<InputDevice*> InputDeviceArray;				///< Array of input devices
		typedef std::vector<InputDeviceFeeder*> InputDeviceFeederArray;	///< Array of input device feeders


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Input devices
		InputDeviceArray		mInputDevices;			///< List of input devices
		KeyboardDevice*			mKeyboardDevice;		///< Keyboard device instance, can be a null pointer if the input system is currently not up-and-running, destroy the instance in case you no longer need it
		MouseDevice*			mMouseDevice;			///< Mouse device instance, can be a null pointer if the input system is currently not up-and-running, destroy the instance in case you no longer need it
		JoystickDevice*			mJoystickDevice;		///< Joystick device instance, can be a null pointer if the input system is currently not up-and-running, destroy the instance in case you no longer need it

		// Input device feeders
		InputDeviceFeederArray	mInputDeviceFeeders;	///< List of input device feeders
		UserInputDeviceFeeder*	mUserInputDeviceFeeder;	///< User input device feeder instance, can be a null pointer if the input system is currently not up-and-running, destroy the instance in case you no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/InputDeviceManager-inl.h"
