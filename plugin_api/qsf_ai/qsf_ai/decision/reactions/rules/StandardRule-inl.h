// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		namespace reactionrules
		{


			// StandardRule inline implementations
			inline StandardRule::StandardRule(EvaluationParameters::PerceptionEventSource eventSource) :
				ReactionRule(),
				mEventId(),
				mEventSource(eventSource)
			{
			}

			inline EvaluationParameters::PerceptionEventIterator StandardRule::checkForEvent(const EvaluationParameters& parameters) const
			{
				return parameters.begin(mEventId,mEventSource);
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // reactionrules
	} // ai
} // qsf
