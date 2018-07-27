// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/script/lua/LuaUserData.h"
#include "qsf/reflection/object/ObjectProxy.h"


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
		*    Lua QSF object pointer base class
		*/
		class LuaObjectPointerBase : public LuaUserData
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
			*    Destructor
			*/
			virtual ~LuaObjectPointerBase();

			/**
			*  @brief
			*    Returns the pointer to the QSF object to wrap
			*
			*  @return
			*    Pointer to the QSF object to wrap, can be a null pointer
			*/
			Object* getObject() const;

			/**
			*  @brief
			*    Comparison operator
			*
			*  @param[in] other
			*    Other instance to compare with
			*
			*  @return
			*    "true" if both are equal, else "false"
			*/
			bool operator ==(const LuaObjectPointerBase& other) const;


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
			explicit LuaObjectPointerBase(Type type = OBJECT_POINTER_TYPE);

			/**
			*  @brief
			*    Initialize this Lua QSF object instance
			*
			*  @param[in] luaScript
			*    The owner Lua script instance
			*  @param[in] object
			*    Pointer to the QSF object to wrap, can be a null pointer
			*/
			void initializeInstance(LuaScript& luaScript, Object* object);

			/**
			*  @brief
			*    De-initializes this instance
			*/
			void deInitializeInstance();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::LuaUserData methods            ]
		//[-------------------------------------------------------]
		protected:
			virtual int indexMetamethod(lua_State* luaState) override;
			virtual int newIndexMetamethod(lua_State* luaState) override;
			virtual void callMetamethod(lua_State* luaState) override;
			virtual void toStringMetamethod(lua_State* luaState) override;
			virtual int equalityMetamethod(lua_State* luaState) override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ObjectProxy mObjectProxy;	///< Object proxy wrapping up the object this Lua object pointer refers to, is automatically updated in case the wrapped up object gets destroyed


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // lua
} // qsf
