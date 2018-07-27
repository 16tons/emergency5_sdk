// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/script/lua/LuaObjectPointerBase.h"


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
namespace qsf
{
	class Script;
}


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
		*    Lua QSF object script function pointer
		*/
		class LuaObjectScriptFunctionPointer : public LuaObjectPointerBase
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class LuaContext;	// Required for efficient instance management


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			LuaObjectScriptFunctionPointer();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaObjectScriptFunctionPointer();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::LuaUserData methods            ]
		//[-------------------------------------------------------]
		protected:
			virtual int indexMetamethod(lua_State* luaState) override;
			virtual int newIndexMetamethod(lua_State* luaState) override;
			virtual void gcMetamethod(lua_State* luaState) override;
			virtual void callMetamethod(lua_State* luaState) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Initialize this instance
			*
			*  @param[in] luaScript
			*    The owner Lua script instance
			*  @param[in] script
			*    Pointer to the QSF script to wrap, can be a null pointer
			*  @param[in] functionName
			*    Name of the function to call
			*/
			void initializeInstance(LuaScript& luaScript, Script* script, const std::string& functionName);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string mFunctionName;	///< Name of the function to call


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf
