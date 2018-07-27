// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		// inline methods ReactionRule::EvaluationParameters
		inline EvaluationParameters::EvaluationParameters(Entity& entity,PerceptionEventQueue* localPerceptionEventQueue,PerceptionEventQueue* globalPerceptionEventQueue,ActionComponent* actionComponent,const JobArguments* jobArguments,ReactionDebugGroup* debugGroup) :
			mEntity(entity),
			mLocalPerceptionEventQueue(localPerceptionEventQueue),
			mGlobalPerceptionEventQueue(globalPerceptionEventQueue),
			mActionComponent(actionComponent),
			mJobArguments(jobArguments),
			mDebugGroup(debugGroup),
			mPriority(0)
		{
		}

		inline Entity& EvaluationParameters::getEntity() const
		{
			return mEntity;
		}

		inline const JobArguments* EvaluationParameters::getJobArguments() const
		{
			return mJobArguments;
		}

		inline PerceptionEventQueue* EvaluationParameters::getLocalPerceptionEventQueue() const
		{
			return mLocalPerceptionEventQueue;
		}

		inline PerceptionEventQueue* EvaluationParameters::getGlobalPerceptionEventQueue() const
		{
			return mGlobalPerceptionEventQueue;
		}

		inline ActionComponent* EvaluationParameters::getActionComponent() const
		{
			return mActionComponent;
		}

		inline ReactionDebugGroup* EvaluationParameters::getDebugGroup() const
		{
			return mDebugGroup;
		}

		inline action::Priority EvaluationParameters::getPriority() const
		{
			return mPriority;
		}

		// inline methods for iterator initialization
		inline EvaluationParameters::PerceptionEventIterator EvaluationParameters::begin(uint32 eventId,PerceptionEventSource eventSource) const
		{
			return EvaluationParameters::PerceptionEventIterator(eventId,eventSource,*this);
		}
		inline EvaluationParameters::PerceptionEventIterator EvaluationParameters::end() const
		{
			return EvaluationParameters::PerceptionEventIterator(*this);
		}

		// inline methods PerceptionEventIterator
		inline EvaluationParameters::PerceptionEventIterator::PerceptionEventIterator(const EvaluationParameters& evalutationParameters) :
			mEventId(0),
			mRealPerceptionEventSource(std::numeric_limits<uint32>::max()),
			mEventSource(DONT_CARE),
			mCurrentEventQueue(nullptr),
			mEvalutationParameters(evalutationParameters)
		{
		}

		inline EvaluationParameters::PerceptionEventIterator::PerceptionEventIterator(uint32 eventId,PerceptionEventSource eventSource,const EvaluationParameters& evalutationParameters) :
			mEventId(eventId),
			mRealPerceptionEventSource(0),
			mEventSource(eventSource),
			mCurrentEventQueue(nullptr),
			mEvalutationParameters(evalutationParameters)
		{
			getNextEventQueue();
		}

		inline const PerceptionEvent& EvaluationParameters::PerceptionEventIterator::operator*() const
		{
			return *mQueueIterator.getPerceptionEvent();
		}

		inline const PerceptionEvent* EvaluationParameters::PerceptionEventIterator::operator->() const
		{
			return mQueueIterator.getPerceptionEvent();
		}

		inline EvaluationParameters::PerceptionEventIterator::operator const PerceptionEvent*() const
		{
			return mQueueIterator.getPerceptionEvent();
		}

		inline const PerceptionEvent* EvaluationParameters::PerceptionEventIterator::getPerceptionEvent() const
		{
			return mQueueIterator.getPerceptionEvent();
		}

		inline bool EvaluationParameters::PerceptionEventIterator::operator==(const PerceptionEventIterator& rho) const
		{
			return mQueueIterator==rho.mQueueIterator;
		}

		inline bool EvaluationParameters::PerceptionEventIterator::operator!=(const PerceptionEventIterator& rho) const
		{
			return mQueueIterator!=rho.mQueueIterator;
		}

		inline EvaluationParameters::PerceptionEventIterator::operator bool() const
		{
			return mQueueIterator;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
