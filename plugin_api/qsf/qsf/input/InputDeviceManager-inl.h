// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline KeyboardDevice& InputDeviceManager::getKeyboard() const
	{
		QSF_CHECK(nullptr != mKeyboardDevice, "No keyboard device, although the pointer should always be valid!", QSF_REACT_THROW);
		return *mKeyboardDevice;
	}

	inline MouseDevice& InputDeviceManager::getMouse() const
	{
		QSF_CHECK(nullptr != mMouseDevice, "No mouse device, although the pointer should always be valid!", QSF_REACT_THROW);
		return *mMouseDevice;
	}

	inline JoystickDevice& InputDeviceManager::getJoystick() const
	{
		QSF_CHECK(nullptr != mJoystickDevice, "No joystick device, although the pointer should always be valid!", QSF_REACT_THROW);
		return *mJoystickDevice;
	}

	inline UserInputDeviceFeeder& InputDeviceManager::getUserInputDeviceFeeder() const
	{
		QSF_CHECK(nullptr != mUserInputDeviceFeeder, "No user input device feeder, although the pointer should always be valid!", QSF_REACT_THROW);
		return *mUserInputDeviceFeeder;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
