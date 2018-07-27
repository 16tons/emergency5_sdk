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

#ifndef __PU_DO_STOP_SYSTEM_EVENT_HANDLER_FACTORY_H__
#define __PU_DO_STOP_SYSTEM_EVENT_HANDLER_FACTORY_H__

#include "ParticleUniversePrerequisites.h"
#include "ParticleUniverseEventHandlerFactory.h"
#include "ParticleUniverseDoStopSystemEventHandler.h"
#include "ParticleUniverseDoStopSystemEventHandlerTokens.h"

namespace ParticleUniverse
{
	/** This factory class is responsible for creation of a DoStopSystemEventHandler.
    */
	class _ParticleUniverseExport DoStopSystemEventHandlerFactory : public ParticleEventHandlerFactory
	{
		public:
			DoStopSystemEventHandlerFactory(void) {};
	        virtual ~DoStopSystemEventHandlerFactory(void) {};

			/** See ParticleEventHandlerFactory*/
			virtual String getEventHandlerType(void) const
			{
				return "DoStopSystem";
			}

			/** See ParticleEventHandlerFactory*/
			virtual ParticleEventHandler* createEventHandler(void)
			{
				return _createEventHandler<DoStopSystemEventHandler>();
			}

			/*  */
			virtual void write(ParticleScriptSerializer* serializer, const IElement* element)
			{
				// Delegate
				mDoStopSystemEventHandlerWriter.write(serializer, element);
			}

		protected:
			DoStopSystemEventHandlerWriter mDoStopSystemEventHandlerWriter;
	};

}
#endif
