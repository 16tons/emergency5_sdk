// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Context.h"
#include "qsf/script/lua/LuaObjectPointer.h"
#include "qsf/script/lua/LuaObjectSlotPointer.h"
#include "qsf/script/lua/LuaObjectMethodPointer.h"
#include "qsf/script/lua/LuaObjectScriptFunctionPointer.h"
#include "qsf/script/lua/LuaObjectSignalPointer.h"
#include "qsf/script/lua/LuaObjectSignalMethodPointer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace lua
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Static Lua context
		*/
		class LuaContext : public Context
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Add a context reference
			*
			*  @note
			*    - If this is the first reference, the internal Lua context is created automatically
			*/
			static void addContextReference();

			/**
			*  @brief
			*    Release a context reference
			*
			*  @note
			*    - If this is the last reference, the internal Lua context is destroyed automatically
			*/
			static void releaseContextReference();

			//[-------------------------------------------------------]
			//[ Lua user data types                                   ]
			//[-------------------------------------------------------]
			// LuaObjectPointer
			static inline LuaObjectPointer& getLuaObjectPointer(LuaScript& luaScript, Object* object);
			static inline void releaseLuaObjectPointer(LuaObjectPointer& luaObjectPointer);
			// LuaObjectSlotPointer	// TODO(co) Port my PixelLight script integration to QSF
			//static inline LuaObjectSlotPointer& getLuaObjectSlotPointer(LuaScript& luaScript, Object* object, PLCore::DynEventHandler *pDynEventHandler);
			//static inline void releaseLuaObjectSlotPointer(LuaObjectSlotPointer& luaObjectSlotPointer);
			// LuaObjectMethodPointer	// TODO(co) Port my PixelLight script integration to QSF
			static inline LuaObjectMethodPointer& getLuaObjectMethodPointer(LuaScript& luaScript, Object* object, const camp::Function& campFunction);
			static inline void releaseLuaObjectMethodPointer(LuaObjectMethodPointer& luaObjectMethodPointer);
			// LuaObjectScriptFunctionPointer
			static inline LuaObjectScriptFunctionPointer& getLuaObjectScriptFunctionPointer(LuaScript& luaScript, Script* script, const std::string& functionName);
			static inline void releaseLuaObjectScriptFunctionPointer(LuaObjectScriptFunctionPointer& luaObjectScriptFunctionPointer);
			// LuaObjectSignalPointer	// TODO(co) Port my PixelLight script integration to QSF
			//static inline LuaObjectSignalPointer& getLuaObjectSignalPointer(LuaScript& luaScript, Object* object, PLCore::DynEvent *pDynEvent);
			//static inline void releaseLuaObjectSignalPointer(LuaObjectSignalPointer& luaObjectSignalPointer);
			// LuaObjectSignalMethodPointer	// TODO(co) Port my PixelLight script integration to QSF
			//static inline LuaObjectSignalMethodPointer& getLuaObjectSignalMethodPointer(LuaScript& luaScript, Object* object, PLCore::DynEvent *pDynEvent, LuaObjectSignalMethodPointer::MethodType nMethod);
			//static inline void releaseLuaObjectSignalMethodPointer(LuaObjectSignalMethodPointer& luaObjectSignalMethodPointer);


		//[-------------------------------------------------------]
		//[ Private static data                                   ]
		//[-------------------------------------------------------]
		private:
			static uint32 mContextCounter;	///< Lua context counter


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/script/lua/LuaContext-inl.h"
