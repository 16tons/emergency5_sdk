// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/script/lua/LuaObjectPointerBase.h"


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
		*    Lua QSF object pointer
		*/
		class LuaObjectPointer : public LuaObjectPointerBase
		{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Pushes an QSF object instance onto the Lua stack
			*
			*  @param[in] luaScript
			*    The owner Lua script instance
			*  @param[in] object
			*    Pointer to the QSF object to wrap, in case of a null pointer nil is pushed onto the Lua stack
			*
			*  @note
			*    - The destruction of the new "qsf::LuaObjectPointer" instance is done by the Lua garbage collector
			*/
			static void luaStackPush(LuaScript& luaScript, Object* object);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			LuaObjectPointer();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaObjectPointer();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::LuaUserData methods            ]
		//[-------------------------------------------------------]
		protected:
			virtual void gcMetamethod(lua_State* luaState) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf
