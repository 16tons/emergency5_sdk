// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/BoostAssertHandler.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	template <class T>
	T* UniqueInstance<T>::mInstance = nullptr;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	template <typename T>
	T* UniqueInstance<T>::getInstance()
	{
		return mInstance;
	}

	template <typename T>
	T& UniqueInstance<T>::getInstanceSafe()
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mInstance, "The unique instance is invalid", QSF_REACT_THROW);
		return *mInstance;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	template <typename T>
	UniqueInstance<T>::UniqueInstance()
	{
		QSF_ASSERT(nullptr == mInstance, "There's already an unique instance", QSF_REACT_THROW);
		mInstance = static_cast<T*>(this);
	}

	template <typename T>
	UniqueInstance<T>::~UniqueInstance()
	{
		mInstance = nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
