/*  Copyright 2010-2012 Matthew Paul Reid
	This file is subject to the terms and conditions defined in
	file 'License.txt', which is part of this source code package.

	Original License.txt content:

	Copyright 2010-2012 Matthew Paul Reid

	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
	documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons
	to whom the Software is furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
	FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
	WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#pragma once

#include <Ogre.h>

namespace Ogre {

class CsmGpuConstants;

class StableCsmShadowCameraSetup : public Ogre::DefaultShadowCameraSetup
{
public:
	typedef std::vector<Ogre::Real> SplitPointList;

public:
	// TODO(oi) Commented out because this is a data passing optimization and it is disabled for cleaness
	StableCsmShadowCameraSetup(/*CsmGpuConstants* constants*/);
	~StableCsmShadowCameraSetup();

	/** Calculate a new splitting scheme.
	@param cascadeCount The number of cascades to use
	@param firstSplitDist distance of the first split
	@param farDist The far plane to use for the last split
	@param lambda Value between 0 (linear splits) and 1 (logarithmic splits)
	*/
	void calculateSplitPoints(size_t cascadeCount, Ogre::Real firstSplitDist, Ogre::Real farDist, Ogre::Real lambda = 0.95);

	/** Manually configure a new splitting scheme.
	@param newSplitPoints A list which is cascadeCount + 1 entries long, containing the
		split points. The first value is the near point, the last value is the
		far point, and each value in between is both a far point of the previous
		split, and a near point for the next one.
	*/
	void setSplitPoints(const SplitPointList& newSplitPoints);

	void setSplitPadding(Ogre::Real pad) { mSplitPadding = pad; }

	/** Get the padding factor to apply to the near & far distances when matching up
		splits to one another, to avoid 'cracks'.
	*/
	Ogre::Real getSplitPadding() const { return mSplitPadding; }
	/// Get the number of splits.
	size_t getCascadeCount() const { return mCascadeCount; }

	/// Returns a stable CSM shadow camera for the given iteration
	virtual void getShadowCamera(const Ogre::SceneManager* sm, const Ogre::Camera* cam,
		const Ogre::Viewport* vp, const Ogre::Light* light, Ogre::Camera* texCam, size_t iteration) const;

	/// Returns the calculated split points.
	inline const SplitPointList& getSplitPoints() const
	{ return mSplitPoints; }

protected:
	void getShadowCameraForCascade(const Ogre::SceneManager* sm, const Ogre::Camera* cam,
		const Ogre::Viewport* vp, const Ogre::Light* light, Ogre::Camera* texCam, size_t iteration,
		Ogre::Real nearSplit, Ogre::Real farSplit) const;

protected:
	size_t mCascadeCount;
	SplitPointList mSplitPoints;
	Ogre::Real mSplitPadding;

	mutable size_t mCurrentIteration;

private:
	// CsmGpuConstants* mGpuConstants;
};

} // namespace Ogre
