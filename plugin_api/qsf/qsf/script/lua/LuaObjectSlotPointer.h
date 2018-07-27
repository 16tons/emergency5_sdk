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
namespace PLCore
{
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
		*    Lua QSF object slot pointer
		*/
		class LuaObjectSlotPointer : public LuaObjectPointerBase
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class LuaContext;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			LuaObjectSlotPointer();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaObjectSlotPointer();

			/**
			*  @brief
			*    Return the pointer to the QSF object slot to wrap
			*
			*  @return
			*    Pointer to the QSF object slot to wrap, can be a null pointer
			*/
			PLCore::DynEventHandler* GetDynEventHandler() const;


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
			PLCore::DynEventHandler* m_pDynEventHandler;	///< Pointer to the QSF object slot to wrap, can be a null pointer


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
			*  @param[in] pDynEventHandler
			*    Pointer to the QSF object slot to wrap, can be a null pointer
			*/
			void initializeInstance(LuaScript& luaScript, Object* object, PLCore::DynEventHandler* pDynEventHandler);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf


// TODO(co) Port my PixelLight script integration to QSF
#endif
