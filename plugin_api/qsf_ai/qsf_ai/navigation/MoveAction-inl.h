// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>

namespace qsf
{
	namespace ai
	{
		inline MoveAction::MoveAction() :
			Action(ACTION_ID),
			mReactionOnFailedGoal(action::RESULT_CLEARLIST),	// Default reaction is to clear the whole action plan
			mMovementMode(getUninitialized<unsigned int>())
		{}

		inline MoveAction::MoveAction(uint32 actionId) :
			Action(actionId),
			mReactionOnFailedGoal(action::RESULT_CLEARLIST),	// Default reaction is to clear the whole action plan
			mMovementMode(getUninitialized<unsigned int>())
		{}

		inline void MoveAction::init(NavigationGoal* goal, unsigned int movementModeId, Path* path)
		{
			QSF_CHECK(goal || path, "Move action without goal and path can't do anything",
				QSF_REACT_NONE);

			mGoal.reset(goal);
			mMovementMode = movementModeId;
			mPath.reset(path);
		}

		inline unsigned int MoveAction::getMovementModeId() const
		{
			return mMovementMode;
		}

		inline const WeakPtr<NavigationComponent>& MoveAction::getNavigationComponent() const
		{
			return mNavigationComponent;
		}

		inline std::auto_ptr<NavigationGoal>& MoveAction::getGoal()
		{
			return mGoal;
		}
	}
}
