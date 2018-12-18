// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InputEvent::InputEvent(InputDevice& device) :
		mDevice(device)
	{
		// Nothing here
	}

	inline InputEvent::~InputEvent()
	{
		// Nothing here
	}

	inline InputDevice& InputEvent::getDevice() const
	{
		return mDevice;
	}

	inline void InputEvent::applyToDevice() const
	{
		// Nothing here
	}

	inline void InputEvent::onAddToQueue() const
	{
		// Nothing here
	}

	template<typename TYPE>
	inline bool InputEvent::isOfType() const
	{
		// Check if own type ID is equal to the type ID of the given class
		return (getTypeId() == TYPE::INPUT_EVENT_ID);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
