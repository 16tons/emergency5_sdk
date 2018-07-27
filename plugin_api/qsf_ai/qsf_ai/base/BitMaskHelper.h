// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>

// This file contains some helper functions for bitmasking
// TODO(vs) replacing this basic operations with longer to write functions can be a problem both regarding plain code textual size as well as performance if the things will not be inlined

namespace qsf
{
	namespace bitmasking
	{
		// Negates a bit pattern
		template <typename BitMask>
		QSF_FORCEINLINE BitMask negate(BitMask mask)
		{
			return ~mask;
		}

		// Forces some bits in a bit pattern to true
		template <typename BitMask, typename Operand>
		QSF_FORCEINLINE void addTo(BitMask& in, Operand toAdd)
		{
			in |= static_cast<BitMask>(toAdd);
		}

		// Forces some bits in a bit pattern to false
		template <typename BitMask, typename Operand>
		QSF_FORCEINLINE void resetIn(BitMask& in, Operand toReset)
		{
			in &= negate(static_cast<BitMask>(toReset));
		}

		// Forces some bits in a bit pattern to the value of enable
		template <typename BitMask, typename Operand>
		QSF_FORCEINLINE void setIn(BitMask& in, Operand operand, bool enable)
		{
			if (enable)
				addTo(in, operand);
			else
				resetIn(in, operand);
		}

		// Checks whether some bits in a bit pattern are set to true.
		// Evaluates to true if all bits in searchedPattern are also set in the currentValue
		template <typename BitMask, typename Comparator>
		QSF_FORCEINLINE bool holdsIn(const BitMask currentValue, const Comparator searchedPattern)
		{
			return (currentValue & static_cast<BitMask>(searchedPattern)) == static_cast<BitMask>(searchedPattern);
		}
	}
}
