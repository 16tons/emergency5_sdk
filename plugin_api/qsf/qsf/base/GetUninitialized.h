// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <limits>

#ifdef max
	#error bad windows header included
#endif


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class StringHash;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	// Provides a default value for uninitialized variables of built-in type
	template <typename T>
	T getUninitialized()
	{
		return std::numeric_limits<T>::max();
	}

	// Version for getting an uninitialized value with increased type safety and less typing by passing the variable for which an uninitialized value is desired
	template <typename T>
	T getUninitialized(const T&)
	{
		return std::numeric_limits<T>::max();
	}

	// Ease-of-use function to set an uninitialized value
	template <typename T>
	void setUninitialized(T& value)
	{
		value = getUninitialized<T>();
	}

	// Ease-of-use function to check for an uninitialized value
	template <typename T>
	bool isUninitialized(const T& value)
	{
		return value == getUninitialized<T>();
	}

	// Ease-of-use function to check for an initialized value
	template <typename T>
	bool isInitialized(const T& value)
	{
		return value != getUninitialized<T>();
	}

	// Declare specializations.
	// The implementations are defined with the specialized type definitions.
	template <>
	StringHash getUninitialized<StringHash>();


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
