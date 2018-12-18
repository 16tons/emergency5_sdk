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
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DebugDrawObject::DebugDrawObject(DebugDrawManager& manager, uint32 id, DebugDrawProxy* proxy) :
		mManager(&manager),
		mId(id),
		mProxy(proxy),
		mOgreSceneNode(nullptr),
		mOgreManualObject(nullptr),
		mIsVisible(true),
		mDrawInClipSpace(false),
		mDrawAsBillboard(false)
	{
		// Nothing to do in here
	}

	inline DebugDrawManager& DebugDrawObject::getDebugDrawManager() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mManager, "The debug draw manager instance is invalid", QSF_REACT_THROW);
		return *mManager;
	}

	inline uint32 DebugDrawObject::getId() const
	{
		return mId;
	}

	inline DebugDrawProxy* DebugDrawObject::getProxy() const
	{
		return mProxy;
	}

	inline bool DebugDrawObject::getDrawInClipSpace() const
	{
		return mDrawInClipSpace;
	}

	inline bool DebugDrawObject::isVisible() const
	{
		return mIsVisible;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
