// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

namespace qsf
{
	namespace ai
	{
		/** Interface for a cost type to be used for search algorithms inside the AI.
		* Basically an instance of this class is usually a thin wrapper around the actual value acting as costs with the required service functionality
		* of indicating uninitialized values and some operators and comparators.
		* This is meant as an illustration for the functions that you need to provide but you can't actually inherit from this class.
		* An example implementation is given for the expected default using UnsignedFloat as value.
		*/
		class Costs
		{
		public:
			// Costs need to be constructable in a unique reserved state signaling uninitialized value
			static Costs getUninitialized();

			// Costs need to be constructable from a pure distance traveled in cases there is a switch between worlds with different costs and the native costs are not available.
			static Costs fromDistance(UnsignedFloat distance);

			// All these functions may decide to take the Costs parameter by value instead of by reference depending on the weight of the object
			//@{
			static bool isValid(const Costs& c); // The result of getUninitialized should not qualify as valid here

			Costs operator +(const Costs& other) const;
			bool operator ==(const Costs& other) const;
			bool operator !=(const Costs& other) const;
			bool operator < (const Costs& other) const;
			bool operator > (const Costs& other) const;
			//@}
		};
	}
}
