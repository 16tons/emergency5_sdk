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
#include "qsf/script/lua/LuaObjectPointerBase.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace PLCore {
	class DynEvent;
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
		*    Lua QSF object signal pointer base class
		*/
		class LuaObjectSignalPointerBase : public LuaObjectPointerBase
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaObjectSignalPointerBase();

			/**
			*  @brief
			*    Return the pointer to the QSF object signal to wrap
			*
			*  @return
			*    Pointer to the QSF object signal to wrap, can be a null pointer
			*/
			PLCore::DynEvent* GetDynEvent() const;


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
			explicit LuaObjectSignalPointerBase(Type type = OBJECT_SIGNAL_POINTER_TYPE);

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
			*/
			void initializeInstance(LuaScript& luaScript, Object* object, PLCore::DynEvent *pDynEvent);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::LuaUserData methods            ]
		//[-------------------------------------------------------]
		protected:
			virtual int indexMetamethod(lua_State* luaState) override;
			virtual int newIndexMetamethod(lua_State* luaState) override;
			virtual void callMetamethod(lua_State* luaState) override;


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			PLCore::DynEvent *m_pDynEvent;	///< Pointer to the QSF object signal to wrap, can be a null pointer


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf


// TODO(co) Port my PixelLight script integration to QSF
#endif
