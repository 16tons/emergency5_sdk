// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once

namespace qsf
{
	/**
	* A static size bitset with minimized memory footprint using a char array.
	*/
	template <uint32 Bits>
	class CharArrayBitSet
	{
	public:
		CharArrayBitSet(bool initialValue = false);

		bool get(uint32 index) const;
		void set(uint32 index, bool value);

	private:
		static const uint32 NUM_BYTES = (Bits + 7) / 8;
		unsigned char mBits[NUM_BYTES];
	};
}

#include "qsf/base/manager/CharArrayBitSet-inl.h"
