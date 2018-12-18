// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/logic/action/Action.h"
#include "qsf/logic/action/ActionInsertionMode.h"
#include "qsf/math/EnumBitmask.h"

#include <boost/noncopyable.hpp>

#include <memory>
#include <deque>


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
	*    Action plan class
	*
	*  @remarks
	*    An action plan is a list of actions that are scheduled to be executed by some entity.
	*    The current simple implementation only allows to execute the first action and insert new actions at the back of the plan.
	*    It manages the lifecycle of actions that were added to it.
	*/
	class QSF_API_EXPORT ActionPlan : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::deque<Action*> ActionQueue;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline ActionPlan();
		inline ~ActionPlan();

		inline bool isEmpty() const;

		inline const ActionQueue& getActions() const;

		// Throws an exception if the plan is empty.
		// Do not destroy the returned instance.
		//@{
		Action& getCurrentAction() const;
		//@}

		// Inserts an action into the plan and transfers ownership of the action
		void insert(Action& newAction, action::InsertionMode mode);
		void clear();

		Action& beginActionUpdate();
		void endActionUpdate();

		bool removeActionByReference(Action& action);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		inline void deleteAction(Action& action);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum ProcessedActionFlag
		{
			PROCESSED_ACTION_DELETE,
			PROCESSED_ACTION_PAUSE
		};
		typedef EnumBitmask<uint8, ProcessedActionFlag> ProcessedActionFlags;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ActionQueue			 mActions;				///< Queue of actions forming the action plan
		Action*				 mProcessedAction;		///< Currently processed action, this one may not be deleted
		ProcessedActionFlags mProcessedActionFlags;	///< State flags for the currently processed action
		bool				 mClearPlanInProgress;	///< "true" if we're currently clearing the action plan


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/logic/action/ActionPlan-inl.h"
