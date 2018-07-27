// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/script/Script.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
typedef struct lua_State lua_State;


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
		*    Lua (http://www.lua.org/) script implementation
		*/
		class LuaScript : public Script
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class LuaObjectSignalMethodPointer;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			LuaScript();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaScript();

			/**
			*  @brief
			*    Return the Lua state
			*
			*  @return
			*    Lua state, can be a null pointer, do not destroy the instance
			*/
			lua_State* getLuaState() const;

			/**
			*  @brief
			*    Write the current Lua stack content into the log
			*
			*  @note
			*    - For debugging
			*/
			void luaStackDump();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::Script methods                    ]
		//[-------------------------------------------------------]
		public:
			//[-------------------------------------------------------]
			//[ Global functions                                      ]
			//[-------------------------------------------------------]
			virtual bool isGlobalFunction(const std::string& name, const std::string& namespaceName = "") override;
			virtual bool addGlobalFunction(Object& object, const std::string& functionName, const camp::Function& campFunction, const std::string& namespaceName = "") override;
			virtual bool removeAllGlobalFunctions() override;

			//[-------------------------------------------------------]
			//[ Script source code                                    ]
			//[-------------------------------------------------------]
			virtual std::string getSourceCode() const override;
			virtual bool setSourceCode(const std::string& sourceCode) override;
			virtual void getAssociatedFilenames(std::vector<std::string>& filenames) override;
			virtual bool execute(const std::string& sourceCode) override;

			//[-------------------------------------------------------]
			//[ Global variables                                      ]
			//[-------------------------------------------------------]
			virtual void getGlobalVariables(std::vector<std::string>& globalVariables, const std::string& namespaceName = "") override;
			virtual bool isGlobalVariable(const std::string& name, const std::string& namespaceName = "") override;
			virtual camp::Type getGlobalVariableCampType(const std::string& sName, const std::string& namespaceName = "") override;
			virtual camp::Value getGlobalVariable(const std::string& name, const std::string& namespaceName = "") override;
			virtual void setGlobalVariable(const std::string& name, const camp::Value& campValue, const std::string& namespaceName = "") override;

			//[-------------------------------------------------------]
			//[ Global function call, used by "qsf::ScriptFunction"   ]
			//[-------------------------------------------------------]
			virtual bool beginCall(const std::string& functionName, bool functionResult, const std::string& namespaceName = "") override;
			virtual void pushArgument(bool value) override;
			virtual void pushArgument(float value) override;
			virtual void pushArgument(double value) override;
			virtual void pushArgument(int8 value) override;
			virtual void pushArgument(int16 value) override;
			virtual void pushArgument(int32 value) override;
			virtual void pushArgument(int64 value) override;
			virtual void pushArgument(uint8 value) override;
			virtual void pushArgument(uint16 value) override;
			virtual void pushArgument(uint32 value) override;
			virtual void pushArgument(uint64 value) override;
			virtual void pushArgument(const std::string& string) override;
			virtual void pushArgument(const glm::vec3& value) override;	// TODO(co) Rethink this, in general, the script interface can only interact with primitive data types, no good idea to try to add all complex ones without a generic solution
			virtual void pushArgument(Object* object) override;
			virtual void pushArgument(Object& object) override;
			virtual bool endCall() override;
			virtual bool getReturn(bool value) override;
			virtual float getReturn(float value) override;
			virtual double getReturn(double value) override;
			virtual int8 getReturn(int8 value) override;
			virtual int16 getReturn(int16 value) override;
			virtual int32 getReturn(int32 value) override;
			virtual int64 getReturn(int64 value) override;
			virtual uint8 getReturn(uint8 value) override;
			virtual uint16 getReturn(uint16 value) override;
			virtual uint32 getReturn(uint32 value) override;
			virtual uint64 getReturn(uint64 value) override;
			virtual std::string getReturn(std::string value) override;
			virtual Object* getReturn(Object* value) override;
			virtual Object& getReturn(Object& value) override;


		//[-------------------------------------------------------]
		//[ Private static Lua callback methods                   ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Lua function callback
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @return
			*    Number of parameters to return to Lua
			*/
			static int luaFunctionCallback(lua_State* luaState);


		//[-------------------------------------------------------]
		//[ Private structures                                    ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    A global function
			*/
			struct GlobalFunction
			{
				LuaScript*			  luaScript;		///< Pointer to the owner script instance, always valid
				Object*				  object;			///< Object to apply the function on, always valid, the instance must stay valid as long as the global function is registered inside the script
				std::string			  functionName;		///< Function name used inside the script to call the global function
				const camp::Function* campFunction;		///< CAMP function to be called, always valid
				std::string			  namespaceName;	///< Optional namespace (e.g. "test", "test.me" and so on)
			};

			/**
			*  @brief
			*    The structure is used to connect script functions with QSF signals
			*/
			struct EventUserData
			{
				// TODO(co) Port my PixelLight script integration to QSF: The CAMP reflection system has no direct support for signals, this was quite handy in PixelLight
				//PLCore::DynEventHandler *pDynEventHandler;		///< The generic event handler, always valid! (delete the instance when no longer required)
				Script*					 script;				///< The owner script instance, always valid!
				int						 luaFunctionReference;	///< The Lua-function or C-function to be called, never LUA_NOREF (use luaL_unref(<LuaState>, LUA_REGISTRYINDEX, <Reference>) when no longer required)
			};


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
			explicit LuaScript(const LuaScript& source);

			/**
			*  @brief
			*    Copy operator
			*
			*  @param[in] cSource
			*    Source to copy from
			*
			*  @return
			*    Reference to this instance
			*/
			LuaScript& operator =(const LuaScript& source);

			/**
			*  @brief
			*    Report Lua errors
			*
			*  @note
			*    - Do only call this method if "mLuaState" is valid and there was in fact an error
			*/
			void reportErrors();

			/**
			*  @brief
			*    Clear the script
			*/
			void clear();

			/**
			*  @brief
			*    Create a nested Lua table
			*
			*  @param[in] tableName
			*    Lua table name (e.g. "test", "test.me" and so on), if empty string, the global Lua table is pushed onto the Lua stack
			*
			*  @note
			*    - Creates all required subtables
			*    - Leaves the deepest table on the Lua stack
			*    - Already existing Lua tables are not overwritten
			*    - "mLuaState" must be valid
			*
			*  @return
			*    "true" if all went fine, else "false" (error within the given Lua table name?)
			*/
			bool createNestedTable(const std::string& tableName);

			/**
			*  @brief
			*    Get a nested Lua table
			*
			*  @param[in] tableName
			*    Lua table name (e.g. "test", "test.me" and so on), if empty string, the global Lua table is pushed onto the Lua stack
			*
			*  @note
			*    - Leaves the deepest table on the Lua stack
			*    - "mLuaState" must be valid
			*
			*  @return
			*    "true" if all went fine, else "false" (error within the given Lua table name?)
			*/
			bool getNestedTable(const std::string& tableName);

			//[-------------------------------------------------------]
			//[ Event and event handler stuff                         ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return event user data key
			*
			*  @param[in] dynEvent
			*    Dynamic event, must be valid
			*  @param[in] luaPointer
			*    Lua pointer to the function, must be valid
			*
			*  @return
			*    Event user data key
			*/
			// TODO(co) Port my PixelLight script integration to QSF: The CAMP reflection system has no direct support for signals, this was quite handy in PixelLight
			//std::string getEventUserDataKey(PLCore::DynEvent* dynEvent, const void* luaPointer) const;

			/**
			*  @brief
			*    Return event user data
			*
			*  @param[in] dynEvent
			*    Dynamic event, must be valid
			*  @param[in] luaPointer
			*    Lua pointer to the function, must be valid
			*
			*  @return
			*    Event user data, can be a null pointer
			*/
			// TODO(co) Port my PixelLight script integration to QSF: The CAMP reflection system has no direct support for signals, this was quite handy in PixelLight
			//EventUserData* getEventUserData(PLCore::DynEvent* dynEvent, const void* luaPointer) const;

			/**
			*  @brief
			*    Add event user data
			*
			*  @param[in] dynEvent
			*    Dynamic event, must be valid
			*  @param[in] luaPointer
			*    Lua pointer to the function, must be valid
			*  @param[in] eventUserData
			*   Event user data to add, must be valid
			*
			*  @note
			*    - Do only call this method if the event user data is not yet added
			*/
			// TODO(co) Port my PixelLight script integration to QSF: The CAMP reflection system has no direct support for signals, this was quite handy in PixelLight
			//void addEventUserData(PLCore::DynEvent* dynEvent, const void* luaPointer, EventUserData* eventUserData);

			/**
			*  @brief
			*    Destroy all registered event user data (a kind of "disconnect all slots at once")
			*/
			void destroyEventUserData();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::string												 mSourceCode;		///< Script source code
			lua_State*												 mLuaState;			///< Lua state, can be a null pointer
			std::string												 mCurrentFunction;	///< Name of the current function
			bool													 mFunctionResult;	///< Has the current function a result?
			uint32													 mCurrentArgument;	///< Current argument, used during function call
			std::vector<GlobalFunction*>							 mGlobalFunctions;	///< List of global functions
			boost::container::flat_map<std::string, EventUserData*>  mEventUserData;	///< Map holding all event user data instances


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::lua::LuaScript)
