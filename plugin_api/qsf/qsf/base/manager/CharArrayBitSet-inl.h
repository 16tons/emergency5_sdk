// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"

namespace qsf
{
	template <uint32 Bits>
	CharArrayBitSet<Bits>::CharArrayBitSet(bool initialValue)
	{
		memset(mBits, initialValue ? -1 : 0, sizeof(unsigned char) * NUM_BYTES);
	}

	template <uint32 Bits>
	bool CharArrayBitSet<Bits>::get(uint32 index) const
	{
		QSF_CHECK(index < Bits, "Index " << index << " out of bounds when accessing bitset",
			QSF_REACT_THROW);

		return (mBits[index / 8] & (1 << (index % 8))) != 0;
	}

	template <uint32 Bits>
	void CharArrayBitSet<Bits>::set(uint32 index, bool value)
	{
		QSF_CHECK(index < Bits, "Index " << index << " out of bounds when accessing bitset",
			QSF_REACT_THROW);

		if (value)
			mBits[index / 8] |= (1 << (index % 8));
		else
			mBits[index / 8] &= ~(1 << (index % 8));
	}
}
