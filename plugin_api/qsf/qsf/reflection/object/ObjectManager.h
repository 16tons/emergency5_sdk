// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <unordered_map>
#include <vector>


//[-------------------------------------------------------]
//[ Forward declaration                                   ]
//[-------------------------------------------------------]
namespace qsf
{
	class Object;
	class ObjectProxy;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Object manager class
	*
	*  @remarks
	*    The object manager is the mediator between object instances and object proxies. In case an object gets destroyed,
	*    the object manager informs all object proxies wrapping up the destroyed object.
	*/
	class QSF_API_EXPORT ObjectManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ClassSystem;	///< Responsible for managing the object manager instance
		friend class ObjectProxy;	///< Only the object manager is allowed to set "qsf::ObjectProxy::mObject"
		friend class Object;		///< Object instances need to inform the object manager in case of destruction


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Default constructor
		*/
		inline ObjectManager();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ObjectManager();

		/**
		*  @brief
		*    Assign an object to an object proxy
		*
		*  @param[in] objectProxy
		*    Object proxy to assign the given object to
		*  @param[in] object
		*    Object to assign to the given object proxy, can be a null pointer
		*/
		void setObjectProxyObject(ObjectProxy& objectProxy, Object* object);

		/**
		*  @brief
		*    This method is called whenever an object proxy gets destroyed
		*
		*  @param[in] objectProxy
		*    Destroyed object proxy instance
		*/
		void invalidateObjectProxy(ObjectProxy& objectProxy);

		/**
		*  @brief
		*    This method is called whenever an object gets destroyed
		*
		*  @param[in] object
		*    Destroyed object instance
		*/
		void objectDestroyed(Object& object);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<ObjectProxy*> ObjectProxies;
		typedef std::unordered_map<Object*, ObjectProxies> ObjectsWithProxies;
		ObjectsWithProxies mObjectsWithProxies;	///< List of object instances which are wrapped by at least one object proxy


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/ObjectManager-inl.h"
