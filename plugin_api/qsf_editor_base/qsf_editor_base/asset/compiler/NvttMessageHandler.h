// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor_base/Export.h"
#include <nvtt/nvtt.h>


//[-------------------------------------------------------]
//[ Interface definitions taken from <nvcore/Debug.h> (this saves us from including half of nvtt's private headers) ]
//[-------------------------------------------------------]
#define NVCORE_API NVTT_API

namespace nv
{
	// Message handler interface.
	struct MessageHandler {
		virtual void log(const char * str, va_list arg) = 0;
		virtual ~MessageHandler() {}
	};

	// Assert handler interface.
	struct AssertHandler {
		virtual int assertion(const char *exp, const char *file, int line, const char *func, const char *msg, va_list arg) = 0;
		virtual ~AssertHandler() {}
	};


	namespace debug
	{
		NVCORE_API void dumpInfo();
		NVCORE_API void dumpCallstack(MessageHandler *messageHandler, int callstackLevelsToSkip = 0);

		NVCORE_API void setMessageHandler(MessageHandler * messageHandler);
		NVCORE_API void resetMessageHandler();

		NVCORE_API void setAssertHandler(AssertHandler * assertHanlder);
		NVCORE_API void resetAssertHandler();

		NVCORE_API void enableSigHandler(bool interactive);
		NVCORE_API void disableSigHandler();

		NVCORE_API bool isDebuggerPresent();
		NVCORE_API bool attachToDebugger();

		NVCORE_API void terminate(int code);
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Structs                                               ]
			//[-------------------------------------------------------]
			struct QSF_EDITOR_BASE_API_EXPORT NvttMessageHandler : public nv::MessageHandler
			{


				//[-------------------------------------------------------]
				//[ Public methods                                        ]
				//[-------------------------------------------------------]
			public:
				NvttMessageHandler();

				~NvttMessageHandler();

				virtual void log(const char * str, va_list arg);


			};

			struct QSF_EDITOR_BASE_API_EXPORT NvttAssertHandler : public nv::AssertHandler
			{


				//[-------------------------------------------------------]
				//[ Public methods                                        ]
				//[-------------------------------------------------------]
			public:
				NvttAssertHandler();

				~NvttAssertHandler();

				// Handler method, note that func might be null pointer!
				virtual int assertion(const char *exp, const char *file, int line, const char *func, const char *msg, va_list arg);


			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
