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
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
namespace camp
{
	class Args;
	class Function;
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
		*    Lua QSF object method pointer
		*/
		class LuaObjectMethodPointer : public LuaObjectPointerBase
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class LuaContext;	// Required for efficient instance management


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return the current Lua function parameters on the Lua stack as CAMP arguments
			*
			*  @param[in]  luaScript
			*    The owner Lua script instance
			*  @param[in]  object
			*    Object to apply the function on
			*  @param[in]  campFunction
			*    CAMP function to be called
			*  @param[in]  isMethod
			*    "true" if the CAMP function is a method, "false" if it's a global function
			*  @param[out] campArgs
			*    Receives the CAMP arguments (the given list is not cleared, new arguments are just added)
			*/
			static void getLuaFunctionParametersAsCampArgs(LuaScript& luaScript, Object& object, const camp::Function& campFunction, bool isMethod, camp::Args& campArgs);

			/**
			*  @brief
			*    Call the current Lua stack CAMP function
			*
			*  @param[in] luaScript
			*    The owner Lua script instance
			*  @param[in] object
			*    Object to apply the function on
			*  @param[in] campFunction
			*    CAMP function to be called
			*  @param[in] isMethod
			*    "true" if the CAMP function is a method, "false" if it's a global function
			*
			*  @return
			*    Number of results on the Lua stack
			*/
			static int callCampFunction(LuaScript& luaScript, Object& object, const camp::Function& campFunction, bool isMethod);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			LuaObjectMethodPointer();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaObjectMethodPointer();

			/**
			*  @brief
			*    Return the CAMP function to be called
			*
			*  @return
			*    CAMP function to be called, can be a null pointer, do not destroy the instance
			*/
			const camp::Function* getCampFunction() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::LuaUserData methods            ]
		//[-------------------------------------------------------]
		protected:
			virtual int indexMetamethod(lua_State* luaState) override;
			virtual int newIndexMetamethod(lua_State* luaState) override;
			virtual void gcMetamethod(lua_State* luaState) override;
			virtual void callMetamethod(lua_State* luaState) override;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			const camp::Function* mCampFunction;	///< CAMP function to be called, can be a null pointer, do not destroy the instance


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
			*  @param[in] object
			*    Pointer to the QSF object to wrap, can be a null pointer
			*  @param[in] campFunction
			*    CAMP function to be called
			*/
			void initializeInstance(LuaScript& luaScript, Object* object, const camp::Function& campFunction);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf
