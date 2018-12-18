// Copyright (C) 2012-2018 Promotion Software GmbH


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
	//[ Helper function definitions                           ]
	//[-------------------------------------------------------]

	// Helper function to read a value from a generic object
	template <typename T>
	bool getValueFromPropertyDictionary(T& variable, const camp::Value& value)
	{
		// Default implementation: let CAMP do the work
		if (value.isCompatible<T>())
		{
			variable = value.to<T>();
			return true;
		}

		// Failed
		return false;
	}


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PropertyDictionary::PropertyDictionary()
	{
		// Nothing here
	}

	inline PropertyDictionary::~PropertyDictionary()
	{
		// Nothing here
	}

	template<typename T>
	void PropertyDictionary::set(const StringHash& identifier, const T& value)
	{
		mValueMap[identifier.getHash()] = value;
	}

	template<typename T>
	bool PropertyDictionary::get(const StringHash& identifier, T& value) const
	{
		// Find the entry
		const ValueMap::const_iterator entry = mValueMap.find(identifier.getHash());
		if (entry == mValueMap.end())
			return false;

		// Convert the value to the correct type
		if (!getValueFromPropertyDictionary(value, entry->second))
		{
			// Failed
			QSF_WARN("Unable to convert value for key " << identifier.getHash() << " because types are not compatible.", QSF_REACT_NONE);
			return false;
		}

		// Success
		return true;
	}

	template<typename T>
	T PropertyDictionary::getSafe(const StringHash& identifier) const
	{
		T value;
		if (!get(identifier, value))
		{
			QSF_ERROR("Could not find value for key " << identifier.getHash() << " in generic object.", QSF_REACT_THROW);
		}
		return value;
	}

	template<typename T>
	T PropertyDictionary::getOptional(const StringHash& identifier, const T& defaultValue) const
	{
		// Find the entry
		const ValueMap::const_iterator entry = mValueMap.find(identifier.getHash());
		if (entry == mValueMap.end())
			return defaultValue;

		// Convert the value to the correct type
		T value;
		if (!getValueFromPropertyDictionary(value, entry->second))
		{
			// Failed
			QSF_WARN("Unable to convert value for key " << identifier.getHash() << " because types are not compatible.", QSF_REACT_NONE);
			return defaultValue;
		}

		// Success
		return value;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
