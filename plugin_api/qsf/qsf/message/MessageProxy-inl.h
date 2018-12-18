// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MessageProxy::MessageProxy() :
		mMessageManager(nullptr),
		mListenerId(getUninitialized<uint32>())
	{
		// Nothing to do in here
	}

	inline MessageProxy::~MessageProxy()
	{
		// Unregister at manager (if registered at all)
		unregister();
	}

	inline bool MessageProxy::isValid() const
	{
		return isInitialized(mListenerId);
	}

	inline MessageManager* MessageProxy::getMessageManager() const
	{
		return mMessageManager;
	}

	inline uint32 MessageProxy::getId() const
	{
		return mListenerId;
	}

	inline bool MessageProxy::unregister()
	{
		// It is ok to try to unregister a message that has not been registered;
		// e.g. because the destructor calls this method
		if (nullptr == mMessageManager)
			return false;

		// Unregister at the message manager
		return unregisterInternal();
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline void MessageProxy::invalidate()
	{
		// Reset internal state
		mMessageManager = nullptr;
		setUninitialized(mListenerId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
