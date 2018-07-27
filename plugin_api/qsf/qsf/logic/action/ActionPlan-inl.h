// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ActionPlan::ActionPlan() :
		mProcessedAction(nullptr),
		mProcessedActionFlags(0),
		mClearPlanInProgress(false)
	{
		// Nothing here
	}

	inline ActionPlan::~ActionPlan()
	{
		clear();
	}

	inline bool ActionPlan::isEmpty() const
	{
		return mActions.empty();
	}

	inline const ActionPlan::ActionQueue& ActionPlan::getActions() const
	{
		return mActions;
	}

	inline void ActionPlan::deleteAction(Action& action)
	{
		// First shutdown the action
		action.shutdown();

		// Now delete it
		delete &action;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
