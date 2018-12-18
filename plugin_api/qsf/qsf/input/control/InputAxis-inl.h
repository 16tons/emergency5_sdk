// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InputAxis::InputAxis(InputDevice& device, const std::string& name, const std::string& description) :
		InputControl(device, INPUT_CONTROL_ID, name, description),
		mValue(0.0f)
	{
		// Nothing here
	}

	inline InputAxis::~InputAxis()
	{
		// Nothing here
	}

	inline float InputAxis::getValue() const
	{
		// Return value
		return mValue;
	}

	inline void InputAxis::setValue(float value)
	{
		// Set value
		mValue = value;
	}

	inline float InputAxis::operator()() const
	{
		return mValue;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputControl methods              ]
	//[-------------------------------------------------------]
	inline void InputAxis::reset()
	{
		mValue = 0.0f;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
