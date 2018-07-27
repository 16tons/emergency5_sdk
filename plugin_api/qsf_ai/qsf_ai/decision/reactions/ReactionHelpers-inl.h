// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		// PlanActions inline implementations
		inline PlanActions::PlanActions(const EvaluationParameters& parameters,const PerceptionEvent* evt) :
			mActionComponent(parameters.getActionComponent()),
			mEvent(evt),
			mPriority(parameters.getPriority()),
			mClearPlan(false)
		{
		}

		inline PlanActions::PlanActions(const EvaluationParameters& parameters,const EvaluationParameters::PerceptionEventIterator& eventIterator) :
			mActionComponent(parameters.getActionComponent()),
			mEvent(eventIterator.getPerceptionEvent()),
			mPriority(parameters.getPriority()),
			mClearPlan(false)
		{
		}

		inline PlanActions::PlanActions(const Entity& entity,const PerceptionEvent* evt,action::Priority priority) :
			mActionComponent(entity.getComponent<ActionComponent>()),
			mEvent(evt),
			mPriority(priority),
			mClearPlan(false)
		{
		}

		inline void PlanActions::clear()
		{
			mActionComponent->clearPlan();
			mClearPlan=true;
		}

		inline action::Priority PlanActions::getPriority() const
		{
			return mPriority;
		}

		inline void PlanActions::setPriority(action::Priority priority)
		{
			mPriority=priority;
		}

		inline bool PlanActions::isCurrentlyEmpty() const
		{
			return mActionComponent->getPlan().isEmpty();
		}

		inline action::Priority PlanActions::getCurrentActionsPriority() const
		{
			return mActionComponent->getCurrentPriority();
		}

		inline PlanActions::operator bool() const
		{
			return (nullptr != mActionComponent);
		}

		template<typename ActionType> inline ActionType& PlanActions::addAction()
		{
			action::InsertionMode insertMode = mClearPlan ? action::APPEND_TO_BACK : action::INSERT_BEFORE_CURRENT;
			// no nullptr check. It is supposed to be checked before!
			return this->mActionComponent->pushAction<ActionType>(mPriority,insertMode);
/*
			// no nullptr check. It is supposed to be checked before!
			ActionType* action=static_cast<ActionType*>(ActionComponent::createActionById(ActionType::ACTION_ID));
			QSF_CHECK(action, "Can't create Action: \"" + ActionType::ACTION_ID.getName() + '\"', QSF_REACT_THROW);
			if (mClearPlan)
			{
				// if the action-plan is cleared then simply appending new actions is fine.
				mParameters.getActionComponent()->insertAction(*action, mPriority, action::APPEND_TO_BACK);
			}
			else
			{
				// otherwise the new actions are first cached and are added in the correct order at PlanAction's destruction.
				mActions.push_back(action);
			}
			return *action;
*/
		}


		// MinMaxDelay inline implementations
		inline MinMaxDelay::MinMaxDelay() :
			mMinDelay(0.0f),
			mMaxDelay(0.0f)
		{
		}

		inline bool MinMaxDelay::isUsed() const
		{
			return mMaxDelay>0.0f;
		}


		// MinMaxDuration inline implementations
		inline MinMaxDuration::MinMaxDuration() :
			mMinDuration(0.0f),
			mMaxDuration(0.0f)
		{
		}

		inline MinMaxDuration::MinMaxDuration(UnsignedFloat minDuration,UnsignedFloat maxDuration) :
			mMinDuration(minDuration),
			mMaxDuration(maxDuration)
		{
		}

		inline void MinMaxDuration::init(UnsignedFloat minDuration,UnsignedFloat maxDuration)
		{
			mMinDuration=minDuration;
			mMaxDuration=maxDuration;
		}

		inline bool MinMaxDuration::isUsed() const
		{
			return mMaxDuration>0.0f;
		}

		// TurnToPercever inline implementations
		inline TurnToPerceiver::TurnToPerceiver() :
			mTurnReverse(false)
		{
		}

		inline bool TurnToPerceiver::getTurnReverse() const
		{
			return mTurnReverse;
		}


		// Animation inline implementations
		inline const std::string& Animation::getAnimationPath() const
		{
			return mAnimationPath;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
