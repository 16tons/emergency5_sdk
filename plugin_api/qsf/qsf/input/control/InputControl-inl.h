// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InputControl::~InputControl()
	{
		// Nothing here
	}

	inline InputDevice& InputControl::getDevice() const
	{
		// Return device
		return *mDevice;
	}

	inline uint32 InputControl::getTypeId() const
	{
		// Return type ID
		return mTypeId;
	}

	inline uint32 InputControl::getId() const
	{
		// Return ID
		return mId;
	}

	inline const std::string& InputControl::getName() const
	{
		// Return name
		return mName;
	}

	inline const std::string& InputControl::getDescription() const
	{
		// Return description
		return mDescription;
	}

	inline bool InputControl::hasName(const StringHash& name) const
	{
		return (getId() == name.getHash());
	}

	inline bool InputControl::operator==(const StringHash& name) const
	{
		return (getId() == name.getHash());
	}

	inline bool InputControl::operator==(const InputControl& control) const
	{
		return (this == &control);
	}

	template<typename TYPE>
	inline bool InputControl::isOfType() const
	{
		// Check if own type ID is equal to the type ID of the given class
		return (getTypeId() == TYPE::INPUT_CONTROL_ID);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputControl methods              ]
	//[-------------------------------------------------------]
	inline void InputControl::reset()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
