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

#ifndef __PU_RENDERER_FACTORY_H__
#define __PU_RENDERER_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseRenderer.h"
#include "ParticleUniverseRendererTokens.h"
#include "ParticleUniverseScriptDeserializer.h"
#include "ParticleUniverseScriptReader.h"

namespace ParticleUniverse
{
	/** This is the base factory of all ParticleRenderer implementations.
    */
	class _ParticleUniverseExport ParticleRendererFactory : public ScriptReader, public ScriptWriter, public FactoryAlloc
	{
	    protected:

			/** 
	        */
			template <class T>
			ParticleRenderer* _createRenderer()
			{
				ParticleRenderer* particleRenderer = PU_NEW_T(T, MEMCATEGORY_SCENE_OBJECTS)();
				particleRenderer->setRendererType(getRendererType());
				return particleRenderer;
			};

	public:
			ParticleRendererFactory(void){};
			virtual ~ParticleRendererFactory(void){};

		    /** Returns the type of the factory, which identifies the particle Renderer type this factory creates. */
			virtual String getRendererType(void) const = 0;

			/** Creates a new renderer instance.
		    @remarks
	        */
			virtual ParticleRenderer* createRenderer() = 0;

			/** Delete a renderer.
	        */
			void destroyRenderer (ParticleRenderer* renderer)
			{
				if (renderer)
					PU_DELETE_T(renderer, ParticleRenderer, MEMCATEGORY_SCENE_OBJECTS);
			};
	};

}
#endif
