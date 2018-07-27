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

#ifndef __PU_BEHAVIOUR_H__
#define __PU_BEHAVIOUR_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseIAlias.h"
#include "ParticleUniverseIElement.h"

namespace ParticleUniverse
{
	/** Defines the behaviour of a particle.
	@remarks
		While a ParticleAffector acts as an external 'force' on the particles, the ParticleBehaviour defines 
		the internal behaviour of each particle individually. For example, 'wind' is typically something that is 
		defined as a ParticleAffector, while 'elasticity' is behaviour of the particle itself.
	@par
		A particle can have multiple Behaviours. Each ParticleBehaviour is accompanied with a specific subclass of 
		the ParticleBehaviourFactory. Similar to ParticleEmitters and ParticleAffectors, each factory is 
		identified by means of a type name.
	@par
		The main reason to introduce the ParticleBehaviour class is to be able to expand the attributes and 
		functions of a particle dynamically and to assign behaviour by means of a particle script.
	*/
	class _ParticleUniverseExport ParticleBehaviour : public IAlias, public IElement
	{
		protected:
			// Type of behaviour
			String mBehaviourType;

			ParticleTechnique* mParentTechnique;

			/** Although the scale is on a Particle System level, the behaviour can also be scaled.
			*/
			Vector3 _mBehaviourScale;

		public:
			ParticleBehaviour(void) : 
				IElement(),
				IAlias(),
				mParentTechnique(0),
				_mBehaviourScale(Vector3::UNIT_SCALE)
			{
				mAliasType = AT_BEHAVIOUR;
			};
			virtual ~ParticleBehaviour(void) {};

			/** Todo
	        */
			const String& getBehaviourType(void) const {return mBehaviourType;};
			void setBehaviourType(const String& behaviourType) {mBehaviourType = behaviourType;};

			/** Todo
	        */
			ParticleTechnique* getParentTechnique(void) const {return mParentTechnique;};
			void setParentTechnique(ParticleTechnique* parentTechnique){mParentTechnique = parentTechnique;};

			/** Notify that the Behaviour is rescaled.
	        */
			virtual void _notifyRescaled(const Vector3& scale){_mBehaviourScale = scale;};

			/** Perform initialisation actions.
			@remarks
				The _prepare() function is automatically called during initialisation (prepare) activities of a 
				ParticleTechnique. A subclass could implement this function to perform initialisation 
				actions.
	        */
			virtual void _prepare(ParticleTechnique* technique){/* No implementation */};

			/** Reverse the actions from the _prepare.
	        */
			virtual void _unprepare(ParticleTechnique* particleTechnique){/* No implementation */};

			/** Perform initialising activities as soon as the particle with which the ParticleBehaviour is
				associated, is emitted.
	        */
			virtual void _initParticleForEmission(Particle* particle){/* No implementation */};

			/** Process a particle.
	        */
			virtual void _processParticle(ParticleTechnique* technique, Particle* particle, Real timeElapsed) = 0;

			/** Perform some action if a particle expires.
	        */
			virtual void _initParticleForExpiration(ParticleTechnique* technique, Particle* particle, Real timeElapsed){/* No implementation */};

			/** Copy attributes to another ParticleBehaviour.
	        */
			virtual void copyAttributesTo (ParticleBehaviour* behaviour)
			{
				copyParentAttributesTo(behaviour);
			};

			/** Copy parent attributes to another behaviour.
	        */
			virtual void copyParentAttributesTo(ParticleBehaviour* behaviour)
			{
				behaviour->mParentTechnique = mParentTechnique;
				behaviour->_mBehaviourScale = _mBehaviourScale;
			};
	};

}
#endif
