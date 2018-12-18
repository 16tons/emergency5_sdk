// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ObjectProxy::ObjectProxy() :
		mObject(nullptr)
	{
		// Nothing to do in here
	}

	inline ObjectProxy::ObjectProxy(const ObjectFunctionBinding& function) :
		mFunctionBinding(function),
		mObject(nullptr)
	{
		// Nothing to do in here
	}

	inline ObjectProxy::ObjectProxy(const ObjectProxy& source) :
		mObject(nullptr)
	{
		setObject(source.mObject);
	}

	inline ObjectProxy::ObjectProxy(Object* object) :
		mObject(nullptr)
	{
		setObject(object);
	}

	inline ObjectProxy& ObjectProxy::operator =(const ObjectProxy& source)
	{
		setObject(source.mObject);
		return *this;
	}

	inline Object* ObjectProxy::getObject() const
	{
		return mObject;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
