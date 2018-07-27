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
		*    Lua QSF object signal pointer
		*/
		class LuaObjectSignalPointer : public LuaObjectSignalPointerBase
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
			LuaObjectSignalPointer();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~LuaObjectSignalPointer();


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


// TODO(co) Port my PixelLight script integration to QSF
#endif
