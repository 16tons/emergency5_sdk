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

#ifndef __PU_LIGHT_RENDERER_H__
#define __PU_LIGHT_RENDERER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseTechnique.h"
#include "OgreSceneNode.h"
#include "OgreLight.h"

namespace ParticleUniverse
{
	/** Visual data specific for this type of renderer.
    */
	class _ParticleUniverseExport LightRendererVisualData : public IVisualData
	{
		public:
			LightRendererVisualData (Ogre::SceneNode* sceneNode) : 
				IVisualData(), 
				node(sceneNode), 
				light(0),
				flashFrequencyCount(0.0f),
				flashLengthCount(0.0f){};
			Ogre::SceneNode* node;
			Ogre::Light* light;
			Real flashFrequencyCount;
			Real flashLengthCount;
			virtual void setVisible(bool visible)
			{
				if (node)
				{
					node->setVisible(visible);
				}
			};
	};

	/** The LightRenderer class is responsible to render particles as a light.
	@remarks
		Note, that the diffuse colour cannot be set. This is, because the light inherits its diffuse colour from the particle. This makes
		it possible to manipulate the colour (for instance, using a Colour Affector).
    */
	class _ParticleUniverseExport LightRenderer : public ParticleRenderer
	{

		protected:
			String mLightName; // Used for random light name prefix
			vector<LightRendererVisualData*> mAllVisualData;
			vector<LightRendererVisualData*> mVisualData;
			vector<Ogre::Light*> mLights;
			size_t mQuota;
			Ogre::Light::LightTypes mLightType;
			ColourValue mSpecularColour;
			Real mAttenuationRange;
			Real mAttenuationConstant;
			Real mAttenuationLinear;
			Real mAttenuationQuadratic;
			Radian mSpotlightInnerAngle;
			Radian mSpotlightOuterAngle;
			Real mSpotlightFalloff;
			Real mPowerScale;
			Real mFlashFrequency;
			Real mFlashLength;
			bool mFlashRandom;

			/** Make all nodes to which the entities are attached visible or invisible.
			*/
			void _makeNodesVisible(bool visible);

		public:
			// Constants
			static const Ogre::Light::LightTypes DEFAULT_LIGHT_TYPE;
			static const ColourValue DEFAULT_DIFFUSE;
			static const ColourValue DEFAULT_SPECULAR;
			static const Real DEFAULT_ATT_RANGE;
			static const Real DEFAULT_ATT_CONSTANT;
			static const Real DEFAULT_ATT_LINEAR;
			static const Real DEFAULT_ATT_QUADRATIC;
			static const Radian DEFAULT_SPOT_INNER_ANGLE;
			static const Radian DEFAULT_SPOT_OUTER_ANGLE;
			static const Real DEFAULT_FALLOFF;
			static const Real DEFAULT_POWER_SCALE;

			LightRenderer();
	        virtual ~LightRenderer(void);

			/** Return the type of light that is emitted.
			*/
			Ogre::Light::LightTypes getLightType(void) const;

			/** Set the type of light that is emitted.
			*/
			void setLightType(Ogre::Light::LightTypes lightType);

			/** 
			*/
			const ColourValue& getSpecularColour(void) const;
			void setSpecularColour(const ColourValue& specularColour);

			/** 
			*/
			Real getAttenuationRange(void) const;
			void setAttenuationRange(Real attenuationRange);

			/** 
			*/
			Real getAttenuationConstant(void) const;
			void setAttenuationConstant(Real attenuationConstant);

			/** 
			*/
			Real getAttenuationLinear(void) const;
			void setAttenuationLinear(Real attenuationLinear);

			/** 
			*/
			Real getAttenuationQuadratic(void) const;
			void setAttenuationQuadratic(Real attenuationQuadratic);

			/** 
			*/
			const Radian& getSpotlightInnerAngle(void) const;
			void setSpotlightInnerAngle(const Radian& spotlightInnerAngle);

			/** 
			*/
			const Radian& getSpotlightOuterAngle(void) const;
			void setSpotlightOuterAngle(const Radian& spotlightOuterAngle);

			/** 
			*/
			Real getSpotlightFalloff(void) const;
			void setSpotlightFalloff(Real spotlightFalloff);

			/** 
			*/
			Real getPowerScale(void) const;
			void setPowerScale(Real powerScale);

			/** 
			*/
			Real getFlashFrequency(void) const;
			void setFlashFrequency(Real flashFrequency);

			/** 
			*/
			Real getFlashLength(void) const;
			void setFlashLength(Real flashLength);

			/** 
			*/
			bool isFlashRandom(void) const;
			void setFlashRandom(bool flashRandom);

			/** Deletes all ChildSceneNodes en Lights.
			*/
			void _destroyAll(void);

			/** @copydoc ParticleRenderer::setVisible */
			//virtual void setVisible(bool visible = true);

			/** @copydoc ParticleRenderer::_prepare */
			virtual void _prepare(ParticleTechnique* technique);

			/** @copydoc ParticleRenderer::_unprepare */
			virtual void _unprepare(ParticleTechnique* technique);

			/** @copydoc ParticleRenderer::_updateRenderQueue */
			//virtual void _updateRenderQueue(Ogre::RenderQueue* queue, Ogre::Camera* camera, const Ogre::Camera* lodCamera, ParticlePool* pool);

			/** @copydoc ParticleRenderer::_processParticle */
			inline virtual void _processParticle(ParticleTechnique* particleTechnique, 
				Particle* particle, 
				Real timeElapsed, 
				bool firstParticle);

			/** @copydoc ParticleRenderer::_setMaterialName */
			virtual void _setMaterialName(const String& materialName);

			/** @copydoc ParticleRenderer::_notifyCurrentCamera */
			virtual void _notifyCurrentCamera(Camera* cam);
	
			/** @copydoc ParticleRenderer::_notifyAttached */
			virtual void _notifyAttached(Ogre::Node* parent);

			/** @copydoc ParticleRenderer::_notifyParticleQuota */
			virtual void _notifyParticleQuota(size_t quota);

			/** @copydoc ParticleRenderer::_notifyDefaultDimensions */
			virtual void _notifyDefaultDimensions(Real width, Real height, Real depth);

			/** @copydoc ParticleRenderer::_notifyParticleResized */
			virtual void _notifyParticleResized(void);

			/** @copydoc ParticleRenderer::_getSortMode */
			virtual SortMode _getSortMode(void) const;

			/** @copydoc ParticleRenderer::copyAttributesTo */
			virtual void copyAttributesTo (ParticleRenderer* renderer);
	};

}
#endif
