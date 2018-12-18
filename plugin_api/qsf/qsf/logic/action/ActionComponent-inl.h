// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ActionComponent::ActionComponent(Prototype* prototype) :
		Component(prototype)
	{
		// Nothing here
	}

	inline ActionComponent::~ActionComponent()
	{
		// Nothing here
	}

	inline const ActionPlan& ActionComponent::getPlan() const
	{
		return mPlan;
	}

	inline Action* ActionComponent::pushActionById(const StringHash& actionTypeId, action::Priority priority, action::InsertionMode insertionMode)
	{
		return pushActionByIdWithParameters(actionTypeId, nullptr, priority, insertionMode);
	}

	template<typename T>
	T& ActionComponent::pushAction(action::Priority priority, action::InsertionMode insertionMode)
	{
		T* action = static_cast<T*>(ActionComponent::createActionById(T::ACTION_ID));
		QSF_CHECK(action, "ActionComponent::PushAction() Can't create action: \"" + T::ACTION_ID.getName() + '\"', QSF_REACT_THROW);
		insertAction(*action, priority, insertionMode);
		return *action;
	}

	inline Action* ActionComponent::getCurrentAction() const
	{
		return mPlan.isEmpty() ? nullptr : &mPlan.getCurrentAction();
	}

	template<typename T>
	T* ActionComponent::getAction() const
	{
		return const_cast<T*>(static_cast<const T*>(getActionByTypeId(T::ACTION_ID)));
	}

	inline bool ActionComponent::removeAction(Action& action)
	{
		return mPlan.removeActionByReference(action);
	}

	inline action::Priority ActionComponent::getCurrentPriority() const
	{
		return mPlan.isEmpty() ? 0 : mPlan.getCurrentAction().getPriority();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	inline bool ActionComponent::onStartup()
	{
		return true;
	}

	inline void ActionComponent::onShutdown()
	{
		// Clear the action plan
		mPlan.clear();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
