// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace lua
	{


		//[-------------------------------------------------------]
		//[ Lua user data types                                   ]
		//[-------------------------------------------------------]
		// LuaObjectPointer
		inline LuaObjectPointer& LuaContext::getLuaObjectPointer(LuaScript& luaScript, Object* object)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			LuaObjectPointer* luaObjectPointer = new LuaObjectPointer();
			luaObjectPointer->initializeInstance(luaScript, object);
			return *luaObjectPointer;
		}

		inline void LuaContext::releaseLuaObjectPointer(LuaObjectPointer& luaObjectPointer)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			delete &luaObjectPointer;
		}

		// LuaObjectSlotPointer	// TODO(co) Port my PixelLight script integration to QSF
		/*
		inline LuaObjectSlotPointer& LuaContext::getLuaObjectSlotPointer(LuaScript& luaScript, Object* object, PLCore::DynEventHandler* pDynEventHandler)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			LuaObjectSlotPointer* luaObjectSlotPointer = new LuaObjectSlotPointer();
			luaObjectPointer->initializeInstance(luaScript, object);
			return *luaObjectSlotPointer;
		}

		inline void LuaContext::releaseLuaObjectSlotPointer(LuaObjectSlotPointer& luaObjectSlotPointer)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			// Remove the given instance (added to the list of free elements)
			delete &luaObjectSlotPointer;
		}*/

		// LuaObjectMethodPointer	// TODO(co) Port my PixelLight script integration to QSF
		inline LuaObjectMethodPointer& LuaContext::getLuaObjectMethodPointer(LuaScript& luaScript, Object* object, const camp::Function& campFunction)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			LuaObjectMethodPointer* luaObjectMethodPointer = new LuaObjectMethodPointer();
			luaObjectMethodPointer->initializeInstance(luaScript, object, campFunction);
			return *luaObjectMethodPointer;
		}

		inline void LuaContext::releaseLuaObjectMethodPointer(LuaObjectMethodPointer& luaObjectMethodPointer)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			// Remove the given instance (added to the list of free elements)
			delete &luaObjectMethodPointer;
		}

		// LuaObjectScriptFunctionPointer
		inline LuaObjectScriptFunctionPointer& LuaContext::getLuaObjectScriptFunctionPointer(LuaScript& luaScript, Script* script, const std::string& functionName)
		{
			// TODO(co) Use "qsf::PagedAllocator"
			LuaObjectScriptFunctionPointer* luaObjectScriptFunctionPointer = new LuaObjectScriptFunctionPointer();
			luaObjectScriptFunctionPointer->initializeInstance(luaScript, script, functionName);
			return *luaObjectScriptFunctionPointer;
		}

		inline void LuaContext::releaseLuaObjectScriptFunctionPointer(LuaObjectScriptFunctionPointer& luaObjectScriptFunctionPointer)
		{
			// TODO(co) Use "qsf::PagedAllocator"
			// Remove the given instance (added to the list of free elements)
			delete &luaObjectScriptFunctionPointer;
		}

		/*
		// LuaObjectSignalPointer	// TODO(co) Port my PixelLight script integration to QSF
		inline LuaObjectSignalPointer& LuaContext::getLuaObjectSignalPointer(LuaScript& luaScript, Object* object, PLCore::DynEvent *pDynEvent)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			LuaObjectSignalPointer* luaObjectSignalPointer = new LuaObjectSignalPointer();
			luaObjectSignalPointer->initializeInstance(luaScript, object);
			return *luaObjectSignalPointer;
		}

		inline void LuaContext::releaseLuaObjectSignalPointer(LuaObjectSignalPointer& luaObjectSignalPointer)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			// Remove the given instance (added to the list of free elements)
			delete &luaObjectSignalPointer;
		}

		// LuaObjectSignalMethodPointer	// TODO(co) Port my PixelLight script integration to QSF
		inline LuaObjectSignalMethodPointer& LuaContext::getLuaObjectSignalMethodPointer(LuaScript& luaScript, Object* object, PLCore::DynEvent *pDynEvent, LuaObjectSignalMethodPointer::MethodType nMethod)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			LuaObjectSignalMethodPointer* luaObjectSignalMethodPointer = new LuaObjectSignalMethodPointer();
			luaObjectSignalMethodPointer->initializeInstance(luaScript, object);
			return *luaObjectSignalMethodPointer;
		}

		inline void LuaContext::releaseLuaObjectSignalMethodPointer(LuaObjectSignalMethodPointer& luaObjectSignalMethodPointer)
		{
			// TODO(co) Port my PixelLight script integration to QSF: Use "qsf::PagedAllocator"
			// Remove the given instance (added to the list of free elements)
			delete &luaObjectSignalMethodPointer;
		}
		*/


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf
