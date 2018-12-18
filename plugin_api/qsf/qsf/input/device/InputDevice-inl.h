// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InputDevice::InputDevice(const std::string& name, const std::string& description) :
		mName(name),
		mDescription(description),
		mRegisteredAtManager(nullptr)
	{
		// Nothing here
	}

	inline const std::string& InputDevice::getName() const
	{
		return mName;
	}

	inline const std::string& InputDevice::getDescription() const
	{
		return mDescription;
	}

	inline const std::vector<InputControl*>& InputDevice::getControls() const
	{
		return mControls;
	}

	inline const std::vector<InputButton*>& InputDevice::getButtons() const
	{
		return mButtons;
	}

	inline const std::vector<InputAxis*>& InputDevice::getAxes() const
	{
		return mAxes;
	}

	inline void InputDevice::updateDevice()
	{
		// Nothing here
	}

	template<typename TYPE>
	inline TYPE* InputDevice::getByName(const StringHash& name) const
	{
		// Try to find the control by its name
		InputControl* control = getControlByName(name);
		if (nullptr == control)
			return nullptr;

		// Does it have the correct type and can be casted safely?
		return control->isOfType<TYPE>() ? static_cast<TYPE*>(control) : nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
