/*
-----------------------------------------------------------------------------------------------
Copyright (C) 2013 Henry van Merode. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------
*/

#ifndef __PU_RADIX_SORT_H__
#define __PU_RADIX_SORT_H__

#include "OgreRadixSort.h"

namespace ParticleUniverse
{
	// If the Ogre renderer is replaced by another renderer, the RadixSort class must be re-implemented
	template <class TContainer, class TContainerValueType, typename TCompValueType>	class RadixSort : 
		public Ogre::RadixSort<TContainer, TContainerValueType, TCompValueType>{};

	typedef Ogre::SortMode SortMode;
	static const SortMode SM_DIRECTION = Ogre::SM_DIRECTION;
	static const SortMode SM_DISTANCE = Ogre::SM_DISTANCE;
}

#endif
