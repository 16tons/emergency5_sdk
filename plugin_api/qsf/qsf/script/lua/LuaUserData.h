// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct lua_State lua_State;
namespace camp
{
	class Value;
}
namespace qsf
{
	namespace lua
	{
		class LuaScript;
	}
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
		*    Abstract Lua user data
		*
		*  @note
		*    - "Lua 5.1 Reference Manual" -> "2.8 - Metatables" -> http://www.lua.org/manual/5.1/manual.html
		*/
		class LuaUserData
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Lua user data types
			*/
			enum Type
			{
				OBJECT_POINTER_TYPE					= 0,	///< Object pointer
				OBJECT_METHOD_POINTER_TYPE			= 1,	///< Object method pointer
				OBJECT_SCRIPT_FUNCTION_POINTER_TYPE	= 2,	///< Object script function pointer
				OBJECT_SIGNAL_POINTER_TYPE			= 3,	///< Object signal pointer
				OBJECT_SIGNAL_METHOD_POINTER_TYPE	= 4,	///< Object signal method pointer
				OBJECT_SLOT_POINTER_TYPE			= 5		///< Object slot pointer
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Create the metatable for "qsf::LuaUserData" events
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*/
			static void createMetatable(lua_State* luaState);

			/**
			*  @brief
			*    Destroy the metatable for "qsf::LuaUserData" events
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*/
			static void destroyMetatable(lua_State* luaState);

			/**
			*  @brief
			*    Return user data from the Lua stack without removing it
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*  @param[in] index
			*    Lua stack index
			*
			*  @return
			*    The user data, null pointer on error, do not destroy the instance
			*/
			static LuaUserData* getUserDataFromLuaStack(lua_State* luaState, int index);

			/**
			*  @brief
			*    Return user data from the top of the Lua stack and removes it from the Lua stack
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @return
			*    The user data, null pointer on error, do not destroy the instance
			*/
			static LuaUserData* popUserDataFromLuaStack(lua_State* luaState);

			/**
			*  @brief
			*    Push a CAMP value onto the Lua stack
			*
			*  @param[in] luaScript
			*    The owner Lua script instance, must stay alive as long as the value data is in use
			*  @param[in] campValue
			*    CAMP value to push
			*/
			static void pushCampValueToLuaStack(LuaScript& luaScript, const camp::Value& campValue);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return the Lua user data type
			*
			*  @return
			*    The Lua user data type
			*/
			Type getType() const;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] type
			*    The Lua user data type
			*/
			explicit LuaUserData(Type type);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaUserData();

			/**
			*  @brief
			*    Initializes this instance
			*
			*  @param[in] luaScript
			*    The owner Lua script instance, must stay alive as long as this Lua user data is in use
			*/
			void initializeInstance(LuaScript& luaScript);

			/**
			*  @brief
			*    Copy operator
			*
			*  @param[in] source
			*    Source to copy from
			*
			*  @return
			*    Reference to this instance
			*/
			LuaUserData& operator =(const LuaUserData& source);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::LuaUserData methods            ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Lua __index metamethod callback (for table accesses)
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @return
			*    Number of parameters to return to Lua
			*
			*  @see
			*    - "Programming in Lua " -> "13.4.1 - The __index Metamethod" -> http://www.lua.org/pil/13.4.1.html
			*/
			virtual int indexMetamethod(lua_State* luaState) = 0;

			/**
			*  @brief
			*    Lua __newindex metamethod callback (for table updates)
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @return
			*    Number of parameters to return to Lua
			*
			*  @see
			*    - "Programming in Lua " -> "13.4.1 - The __newindex Metamethod" -> http://www.lua.org/pil/13.4.2.html
			*/
			virtual int newIndexMetamethod(lua_State* luaState) = 0;

			/**
			*  @brief
			*    Lua __gc metamethod callback (finalizer/destructor called by garbage collector)
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @see
			*    - "Programming in Lua " -> "29 - Managing Resources" -> http://www.lua.org/pil/29.html
			*/
			virtual void gcMetamethod(lua_State* luaState) = 0;

			/**
			*  @brief
			*    Lua __call metamethod callback (called when Lua calls a value)
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @see
			*    - "Lua 5.1 Reference Manual" -> "2.8 - Metatables" -> ""call": called when Lua calls a value" -> http://www.lua.org/manual/5.1/manual.html
			*/
			virtual void callMetamethod(lua_State* luaState) = 0;

			/**
			*  @brief
			*    Lua __tostring metamethod callback (called when Lua tries to convert the user data into a string)
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @see
			*    - "Lua 5.1 Reference Manual" -> "5.1 - Basic Functions" -> "If the metatable of e has a "__tostring" field, then tostring calls the corresponding value with e as argument, and uses the result of the call as its result." -> http://www.lua.org/manual/5.1/manual.html
			*/
			virtual void toStringMetamethod(lua_State* luaState) = 0;

			/**
			*  @brief
			*    Lua __eq metamethod callback (called when Lua tries to check for equality)
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @see
			*    - "2.8 - Metatables" -> ""eq": the == operation. The function getcomphandler defines how Lua chooses a metamethod for comparison operators. A metamethod only is selected when both objects being compared have the same type and the same metamethod for the selected operation." -> http://www.lua.org/manual/5.1/manual.html
			*/
			virtual int equalityMetamethod(lua_State* luaState) = 0;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			LuaScript* mLuaScript;	///< The owner Lua script instance, always valid, do not destroy the instance


		//[-------------------------------------------------------]
		//[ Private static Lua callback methods                   ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Lua __index metamethod callback (for table accesses)
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @return
			*    Number of parameters to return to Lua
			*
			*  @see
			*    - "Programming in Lua " -> "13.4.1 - The __index Metamethod" -> http://www.lua.org/pil/13.4.1.html
			*/
			static int luaIndexMetamethodCallback(lua_State* luaState);

			/**
			*  @brief
			*    Lua __newindex metamethod callback (for table updates)
			*
			*  @param[in] luaState
			*    Lua state
			*
			*  @return
			*    Number of parameters to return to Lua
			*
			*  @see
			*    - "Programming in Lua " -> "13.4.1 - The __newindex Metamethod" -> http://www.lua.org/pil/13.4.2.html
			*/
			static int luaNewIndexMetamethodCallback(lua_State* luaState);

			/**
			*  @brief
			*    Lua __gc metamethod callback (finalizer/destructor called by garbage collector)
			*
			*  @param[in] luaState
			*    Lua state
			*
			*  @return
			*    Number of parameters to return to Lua
			*
			*  @see
			*    - "Programming in Lua " -> "29 - Managing Resources" -> http://www.lua.org/pil/29.html
			*/
			static int luaGcMetamethodCallback(lua_State* luaState);

			/**
			*  @brief
			*    Lua __call metamethod callback (called when Lua calls a value)
			*
			*  @param[in] luaState
			*    Lua state
			*
			*  @return
			*    Number of parameters to return to Lua
			*
			*  @see
			*    - "Lua 5.1 Reference Manual" -> "2.8 - Metatables" -> ""call": called when Lua calls a value" -> http://www.lua.org/manual/5.1/manual.html
			*/
			static int luaCallMetamethodCallback(lua_State* luaState);

			/**
			*  @brief
			*    Lua __tostring metamethod callback (called when Lua tries to convert the user data into a string)
			*
			*  @param[in] luaState
			*    Lua state
			*
			*  @return
			*    Number of parameters to return to Lua
			*
			*  @see
			*    - "Lua 5.1 Reference Manual" -> "5.1 - Basic Functions" -> "If the metatable of e has a "__tostring" field, then tostring calls the corresponding value with e as argument, and uses the result of the call as its result." -> http://www.lua.org/manual/5.1/manual.html
			*/
			static int luaToStringMetamethodCallback(lua_State* luaState);

			/**
			*  @brief
			*    Lua __eq metamethod callback (called when Lua tries to check for equality)
			*
			*  @param[in] luaState
			*    Lua state
			*
			*  @return
			*    Number of parameters to return to Lua
			*
			*  @see
			*    - "2.8 - Metatables" -> ""eq": the == operation. The function getcomphandler defines how Lua chooses a metamethod for comparison operators. A metamethod only is selected when both objects being compared have the same type and the same metamethod for the selected operation." -> http://www.lua.org/manual/5.1/manual.html
			*/
			static int luaEqualityMetamethodCallback(lua_State* luaState);


		//[-------------------------------------------------------]
		//[ Private static data                                   ]
		//[-------------------------------------------------------]
		private:
			static char LUA_METATABLE;	///< The address of this static variable is used to identify the metatable of the user data


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Copy constructor
			*
			*  @param[in] source
			*    Source to copy from
			*/
			explicit LuaUserData(const LuaUserData& source);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Type mType;	///< The Lua user data type


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf
