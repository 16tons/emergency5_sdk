// Copyright (C) 2012-2017 Promotion Software GmbH

namespace qsf
{
	namespace ai
	{
		inline void ReactionSystem::updateGlobals(const JobArguments&) const
		{}

		inline void ReactionSystem::createDebugOutput(const ReactionComponent&) const
		{}

		inline void ReactionSystem::pushEvent(const PerceptionEvent& perceptionEvent)
		{
			mGlobalPerceptionEventQueue->push(perceptionEvent);
		}

		inline PerceptionEventQueue& ReactionSystem::getPerceptionEventQueue() const
		{
			return *mGlobalPerceptionEventQueue;
		}

		inline ReactionDebugGroup* ReactionSystem::getDebugSettings() const
		{
			return mDebugSettings;
		}

	}
}
