// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>

#include <glm/detail/func_common.hpp> // for isnan


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		inline UnsignedFloatCosts::UnsignedFloatCosts() :
			mValue(UnsignedFloat::ZERO)
		{}

		inline UnsignedFloatCosts::UnsignedFloatCosts(UnsignedFloat value) :
			mValue(value)
		{}

		inline UnsignedFloatCosts UnsignedFloatCosts::getUninitialized()
		{
			// TODO(vs) If std::numeric_limits<float>::max() is not optimized away we could manually optimize it here to access a static variable here
			return UnsignedFloat(UncheckedUnsignedFloat(qsf::getUninitialized<float>()));
		}

		inline UnsignedFloatCosts UnsignedFloatCosts::fromDistance(UnsignedFloat distance)
		{
			return distance; // converts directly to the mValue
		}

		inline bool UnsignedFloatCosts::isValid(UnsignedFloatCosts costs)
		{
			return isInitialized(*costs.mValue) && !glm::isnan(*costs.mValue) && *costs.mValue != std::numeric_limits<float>::infinity();
		}

		inline UnsignedFloatCosts UnsignedFloatCosts::operator +(UnsignedFloatCosts other) const
		{
			return mValue + other.mValue;
		}

		inline UnsignedFloatCosts UnsignedFloatCosts::operator +=(UnsignedFloatCosts other)
		{
			mValue += other.mValue;
			return *this;
		}

		inline bool UnsignedFloatCosts::operator ==(UnsignedFloatCosts other) const
		{
			return mValue == other.mValue;
		}

		inline bool UnsignedFloatCosts::operator !=(UnsignedFloatCosts other) const
		{
			return !(*this == other);
		}

		inline bool UnsignedFloatCosts::operator < (UnsignedFloatCosts other) const
		{
			return mValue < other.mValue;
		}

		inline bool UnsignedFloatCosts::operator > (UnsignedFloatCosts other) const
		{
			return mValue > other.mValue;
		}

		inline bool UnsignedFloatCosts::operator <=(UnsignedFloatCosts other) const
		{
			return mValue <= other.mValue;
		}

		inline UnsignedFloat UnsignedFloatCosts::operator *() const
		{
			return mValue;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
