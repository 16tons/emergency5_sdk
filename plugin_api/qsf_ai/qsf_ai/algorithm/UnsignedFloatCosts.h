// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/Costs.h"
#include "qsf_ai/base/UnsignedFloat.h"

namespace qsf
{
	namespace ai
	{
		/** Example implementation for the Costs interface for algorithms that uses UnsignedFloat as costs value.
		* getUninitialized<float>() is used as a reserved uninitialized value in this case.
		*/
		class UnsignedFloatCosts
		{
		public:
			UnsignedFloatCosts(); // creates zero cost
			UnsignedFloatCosts(UnsignedFloat value);

			static const UnsignedFloat UNINITIALIZED;

			// Since the core representation is still equal to a basic data type copying should be faster than a reference when used as a parameter
			//@{
			static UnsignedFloatCosts getUninitialized();
			static UnsignedFloatCosts fromDistance(UnsignedFloat distance);
			static bool isValid(UnsignedFloatCosts costs); // NaN as well as float::max (uninitialized) as well as float::inf are not valid

			UnsignedFloatCosts operator +(UnsignedFloatCosts other) const;
			UnsignedFloatCosts operator +=(UnsignedFloatCosts other);
			bool operator ==(UnsignedFloatCosts other) const;
			bool operator !=(UnsignedFloatCosts other) const;
			bool operator < (UnsignedFloatCosts other) const;
			bool operator > (UnsignedFloatCosts other) const;
			bool operator <=(UnsignedFloatCosts other) const;
			UnsignedFloat operator *() const;
			//@}

		private:
			UnsignedFloat mValue; // The final value, not set when == getUninitialized<float>()
		};
	}
}

#include "qsf_ai/algorithm/UnsignedFloatCosts-inl.h"
