// Copyright (C) 2012-2017 Promotion Software GmbH


// TODO(co) Port my PixelLight script integration to QSF
#if 0


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/script/lua/LuaObjectSignalPointerBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class DynParams;
	class DynEventHandler;
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
		*    Lua QSF object build in signal method pointer
		*/
		class LuaObjectSignalMethodPointer : public LuaObjectSignalPointerBase
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class LuaContext;


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Build in method type
			*/
			enum MethodType
			{
				UNKNOWN_METHOD    = -1,	///< Unknown method
				CONNECT_METHOD    = 0,	///< Connect method "connect()"
				DISCONNECT_METHOD = 1	///< Disconnect method "disconnect()"
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Return the build in method defined by a given string
			*
			*  @param[in] methodType
			*    String to return the build in method type from
			*
			*  @return
			*    The build in method type defined by the given string
			*/
			static MethodType stringToMethodType(const std::string& methodType);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			LuaObjectSignalMethodPointer();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaObjectSignalMethodPointer();


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
			MethodType mMethodType;	///< Build in method


		//[-------------------------------------------------------]
		//[ Private static event callback methods                 ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Event callback function
			*
			*  @param[in] cDynParams
			*    Dynamic parameters
			*  @param[in] userData
			*    Pointer to optional user data, can be a null pointer
			*/
			static void eventCallback(PLCore::DynParams& cDynParams, void* userData);


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
			*  @param[in] pDynEvent
			*    Pointer to the QSF object signal to wrap, can be a null pointer
			*  @param[in] methodType
			*    Build in method type
			*/
			void initializeInstance(LuaScript& luaScript, Object* object, PLCore::DynEvent* pDynEvent, MethodType methodType);

			/**
			*  @brief
			*    Return a QSF slot from the Lua stack without removing it
			*
			*  @param[in] luaState
			*    Lua state, must be valid
			*
			*  @return
			*    The valid QSF slot, null pointer on error
			*
			*  @note
			*    - Performs also a signal/slot signature match
			*    - m_pDynEvent must be valid
			*/
			PLCore::DynEventHandler* getSlotFromLuaStack(lua_State* luaState);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf


// TODO(co) Port my PixelLight script integration to QSF
#endif
