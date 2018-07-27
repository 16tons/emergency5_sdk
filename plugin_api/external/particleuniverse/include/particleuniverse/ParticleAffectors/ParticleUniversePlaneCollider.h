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

#ifndef __PU_PLANE_COLLIDER_H__
#define __PU_PLANE_COLLIDER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseBaseCollider.h"
#include "ParticleUniversePlane.h"

namespace ParticleUniverse
{
	/** The PlaneCollider is a plane shape that collides with the particles. The PlaneCollider can only collide 
		with particles that are created within the same ParticleTechnique as where the PlaneCollider is registered.
    */
	class _ParticleUniverseExport PlaneCollider : public BaseCollider
	{
		public:
			// Constants
			static const Vector3 DEFAULT_NORMAL;

			PlaneCollider(void);
	        virtual ~PlaneCollider(void){};

			/** Returns the normal of the plane
			*/
			const Vector3 getNormal(void) const;

			/** Sets the normal of the plane
			*/
			void setNormal(const Vector3& normal);

			/** Notify that the Affector is rescaled.
	        */
			virtual void _notifyRescaled(const Vector3& scale);

			/** 
			*/
			void calculateDirectionAfterCollision(Particle* particle, Real timeElapsed);

			/** @copydoc ParticleAffector::_affect */
			virtual void _affect(ParticleTechnique* particleTechnique, Particle* particle, Real timeElapsed);

			/** @copydoc ParticleAffector::copyAttributesTo */
			virtual void copyAttributesTo (ParticleAffector* affector);

		protected:
			Vector3 mNormal;
			Vector3 mPredictedPosition;
			Plane mPlane;
	};

}
#endif
