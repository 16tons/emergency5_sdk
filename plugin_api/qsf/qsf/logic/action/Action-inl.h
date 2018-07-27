// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Action::~Action()
	{
		// Nothing here
	}

	inline uint32 Action::getTypeId() const
	{
		return mActionTypeId;
	}

	inline ActionComponent& Action::getComponent() const
	{
		QSF_ASSERT(nullptr != mOwnerComponent, "The action component instance is invalid", QSF_REACT_THROW);
		return *mOwnerComponent;
	}

	inline process::State Action::getState() const
	{
		return mState;
	}

	inline action::Priority Action::getPriority() const
	{
		return mPriority;
	}

	inline void Action::setPriority(action::Priority priority)
	{
		mPriority = priority;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline Action::Action(uint32 actionTypeId) :
		mPriority(0),
		mIsScriptAction(false),
		mActionTypeId(actionTypeId),
		mOwnerComponent(nullptr),
		mState(process::WAITING)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	inline bool Action::onStartup()
	{
		return true;
	}

	inline void Action::onShutdown()
	{
		// Nothing here
	}

	inline void Action::onAbort()
	{
		// Nothing here
	}

	inline void Action::onPause()
	{
		// Nothing here
	}

	inline void Action::onResume()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
