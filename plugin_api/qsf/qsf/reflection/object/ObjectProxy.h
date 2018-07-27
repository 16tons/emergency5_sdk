// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/Proxy.h"

#include <boost/function.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Object;
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
	*    Object proxy class
	*
	*  @remarks
	*    Usually, you reference prototypes/entities (derived from "qsf::Object") by using an ID. The same is true for
	*    components (derived from "qsf::Object"). This way there's no issue in case the "referenced" object instance
	*    gets destroyed. There are situations were it's not possible to use such IDs for concrete instances of classes
	*    derived from "qsf::Object". Storing a raw C pointer to "qsf::Object" instances as e.g. member variable of a class
	*    is quite dangerous because this pointer will point to an invalid memory address as soon as the object it's pointing
	*    to gets destroyed.
	*
	*    The object proxy is a solution for the previously explained issue. Although this solution is safe, there's of course
	*    a certain overhead involved in using and informing such a proxy in case the object gets destroyed. So, don't overuse
	*    object proxies and use IDs of derived classes whenever possible.
	*
	*  @note
	*    - The object proxy is similar to a classic smart pointer, but while smart pointers are about handling reference counter,
	*      the object proxy is about informing the proxy about object loss (meaning react instead of manage)
	*/
	class QSF_API_EXPORT ObjectProxy : public Proxy
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ObjectManager;	///< Only the object manager is allowed to set "qsf::ObjectProxy::mObject"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		/** Function binding signature for object callbacks */
		typedef void ObjectBindingSignature(Object* object);

		/** Boost function binding definition for object callbacks */
		typedef boost::function<ObjectBindingSignature> ObjectFunctionBinding;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline ObjectProxy();

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] function
		*    Function to call in case of an object change
		*/
		inline explicit ObjectProxy(const ObjectFunctionBinding& function);

		/**
		*  @brief
		*    Copy constructor
		*
		*  @param[in] source
		*    Source to copy from
		*/
		inline explicit ObjectProxy(const ObjectProxy& source);

		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] object
		*    Object the object proxy should wrap up, can be a null pointer
		*/
		inline explicit ObjectProxy(Object* object);

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~ObjectProxy();

		/**
		*  @brief
		*    Copy operator
		*
		*  @param[in] source
		*    Source to copy from
		*
		*  @return
		*    Reference to this object proxy instance
		*/
		inline ObjectProxy& operator =(const ObjectProxy& source);

		/**
		*  @brief
		*    Return the wrapped up object instance
		*
		*  @return
		*    The wrapped up object instance, can be a null pointer, in case you destroy the instance the object proxy gets informed about this automatically
		*/
		inline Object* getObject() const;

		/**
		*  @brief
		*    Set the object the object proxy should wrap up
		*
		*  @param[in] object
		*    Object the object proxy should wrap up, can be a null pointer
		*/
		void setObject(Object* object);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ObjectFunctionBinding	mFunctionBinding;	///< Function callback bound to this object proxy
		Object*					mObject;			///< Object instance this object proxy is wrapping, can be a null pointer, don't destroy the instance, only the object manager is allowed to set "qsf::ObjectProxy::mObject"


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/reflection/object/ObjectProxy-inl.h"
