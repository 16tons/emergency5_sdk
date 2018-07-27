// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InputButton::InputButton(InputDevice& device, const std::string& name, const std::string& description, uint32 index) :
		InputControl(device, INPUT_CONTROL_ID, name, description),
		mIndex(index),
		mPressed(false),
		mHit(false)
	{
		// Nothing here
	}

	inline InputButton::~InputButton()
	{
		// Nothing here
	}

	inline uint32 InputButton::getIndex() const
	{
		// Return index
		return mIndex;
	}

	inline bool InputButton::isPressed() const
	{
		// Return state
		return mPressed;
	}

	inline void InputButton::setPressed(bool pressed)
	{
		// If the button was previously pressed but now isn't, we received a hit
		mHit = (mPressed && !pressed);

		// Set state
		mPressed = pressed;
	}

	inline bool InputButton::isHit()
	{
		// Return hit-state
		if (!mHit)
		{
			return false;
		}
		else
		{
			mHit = false;
			return true;
		}
	}

	inline bool InputButton::checkHit()
	{
		// Return hit state
		return mHit;
	}

	inline bool InputButton::operator()() const
	{
		// Return state
		return mPressed;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputControl methods              ]
	//[-------------------------------------------------------]
	inline void InputButton::reset()
	{
		mPressed = false;
		mHit = false;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
