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

#ifndef __PU_RIBBONTRAIL_RENDERER_H__
#define __PU_RIBBONTRAIL_RENDERER_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseRenderer.h"
#include "OgreRibbonTrail.h"
#include "OgreSceneNode.h"

namespace ParticleUniverse
{
	/** Visual data specific for this type of renderer.
    */
	class _ParticleUniverseExport RibbonTrailRendererVisualData : public IVisualData
	{
		public:

			//Constructor
			RibbonTrailRendererVisualData(Ogre::SceneNode* sceneNode, Ogre::v1::RibbonTrail* ribbonTrail) :
				IVisualData(), 
				node(sceneNode),
				trail(ribbonTrail),
				addedToTrail(false),
				index(0){};

			Ogre::SceneNode* node;
			bool addedToTrail;
			Ogre::v1::RibbonTrail* trail;
			size_t index;
			virtual void setVisible(bool visible)
			{
				if (visible)
				{
					if (!addedToTrail)
					{
						trail->addNode(node);
						addedToTrail = true;
					}
				}
				else
				{
					if (addedToTrail)
					{
						trail->removeNode(node);
						addedToTrail = false;
					}
				}
			};
	};

	/** The RibbonTrailRenderer class is responsible to render particles as a RibbonTrail.
    */
	class _ParticleUniverseExport RibbonTrailRenderer : public ParticleRenderer, public TechniqueListener
	{
		protected:
			vector<RibbonTrailRendererVisualData*> mAllVisualData;
			vector<RibbonTrailRendererVisualData*> mVisualData;
			size_t mQuota;
			Ogre::v1::RibbonTrail* mTrail;
			String mRibbonTrailName;
			bool mUseVertexColours;
			size_t mMaxChainElements;
			Real mTrailLength;
			Real mTrailWidth;
			bool mRandomInitialColour;
			bool mSetLength;
			bool mSetWidth;
			ColourValue mInitialColour;
			ColourValue mColourChange;
			Ogre::SceneNode* mChildNode;

		public:
			// Constants
			static const bool DEFAULT_USE_VERTEX_COLOURS;
			static const size_t DEFAULT_MAX_ELEMENTS;
			static const Real DEFAULT_LENGTH;
			static const Real DEFAULT_WIDTH;
			static const bool DEFAULT_RANDOM_INITIAL_COLOUR;
			static const ColourValue DEFAULT_INITIAL_COLOUR;
			static const ColourValue DEFAULT_COLOUR_CHANGE;

			RibbonTrailRenderer();
	        virtual ~RibbonTrailRenderer(void);

			/** Notify that the Particle System is rescaled.
	        */
			virtual void _notifyRescaled(const Vector3& scale);

			/** Getters and Setters
			*/
			bool isUseVertexColours(void) const;
			void setUseVertexColours(bool useVertexColours);

			size_t getMaxChainElements(void) const;
			void setMaxChainElements(size_t maxChainElements);

			Real getTrailLength(void) const;
			void setTrailLength(Real trailLength);

			Real getTrailWidth(void) const;
			void setTrailWidth(Real trailWidth);

			bool isRandomInitialColour(void) const;
			void setRandomInitialColour(bool randomInitialColour);

			const ColourValue& getInitialColour(void) const;
			void setInitialColour(const ColourValue& initialColour);

			const ColourValue& getColourChange(void) const;
			void setColourChange(const ColourValue& colourChange);

			/** Deletes all ChildSceneNodes en Entities.
			*/
			void _destroyAll(void);
			
			/** Enable the RibbonTrail
			*/
//			virtual void _notifyStart(void);

			/** Disable the RibbonTrail
			*/
//			virtual void _notifyStop(void);
			
			/** Make all objects visible or invisible.
			*/
			virtual void setVisible(bool visible);

			/** @copydoc ParticleRenderer::_prepare */
			virtual void _prepare(ParticleTechnique* technique);

			/** @copydoc ParticleRenderer::_unprepare */
			virtual void _unprepare(ParticleTechnique* technique);

			/** 
			*/
			virtual void _updateRenderQueue(Ogre::RenderQueue* queue, Ogre::Camera* camera, const Ogre::Camera* lodCamera, ParticlePool* pool);

			/** 
			*/
			virtual void _notifyAttached(Ogre::Node* parent);

			/** @copydoc ParticleRenderer::_setMaterialName */
			virtual void _setMaterialName(const String& materialName);

			/** 
			*/
			virtual void _notifyParticleQuota(size_t quota);

			/** 
			*/
			virtual void _notifyDefaultDimensions(Real width, Real height, Real depth);

			/** 
			*/
			virtual void _notifyParticleResized(void);

			/** 
			*/
			virtual void _notifyParticleZRotated(void);

			/** 
			*/
			virtual void setRenderQueueGroup(uint8 queueId);
			virtual void setRenderQueueSubGroup(uint8 queueId);

			/** 
			*/
			virtual SortMode _getSortMode(void) const;

			/** 
			*/
			virtual void copyAttributesTo (ParticleRenderer* renderer);

			/*  See TechniqueListener.
			*/
			virtual void particleEmitted(ParticleTechnique* particleTechnique, Particle* particle);

			/*  See TechniqueListener.
			*/
			virtual void particleExpired(ParticleTechnique* particleTechnique, Particle* particle);
	};

}
#endif

