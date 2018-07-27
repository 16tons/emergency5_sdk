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
	inline InputSystem::InputSystem() :
		mNativeWindowHandle(NULL_HANDLE),
		mInputDeviceManager(nullptr),
		mOisManager(nullptr)
	{
		// Nothing to do in here
	}

	inline InputSystem::~InputSystem()
	{
		// Nothing to do in here
	}

	inline InputDeviceManager& InputSystem::getInputDeviceManager() const
	{
		QSF_CHECK(nullptr != mInputDeviceManager, "No input device manager, although the pointer should always be valid!", QSF_REACT_THROW);
		return *mInputDeviceManager;
	}

	inline OisManager* InputSystem::getOisManager() const
	{
		return mOisManager;
	}

	inline KeyboardDevice& InputSystem::getKeyboard() const
	{
		return getInputDeviceManager().getKeyboard();
	}

	inline MouseDevice& InputSystem::getMouse() const
	{
		return getInputDeviceManager().getMouse();
	}

	inline JoystickDevice& InputSystem::getJoystick() const
	{
		return getInputDeviceManager().getJoystick();
	}

	inline UserInputDeviceFeeder& InputSystem::getUserInputDeviceFeeder() const
	{
		return getInputDeviceManager().getUserInputDeviceFeeder();
	}

	template <typename TYPE>
	TYPE& InputSystem::createInputHandler(const StringHash& id)
	{
		// Create a new input handler with the given ID
		TYPE* inputHandler = new TYPE(id);

		// Initialize the handler
		// With this call derived classes can implement special initialize features
		inputHandler->initialize();

		// Add it internally
		addInputHandler(*inputHandler);

		// Return the newly created input handler
		return *inputHandler;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* InputSystem::getName() const
	{
		return "Input system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
