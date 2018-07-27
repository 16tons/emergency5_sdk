// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		// inline implementation
		inline ReactionComponent::ReactionComponent(Prototype* prototype) :
			Component(prototype),
			mMinPriority(DEFAULT_MIN_PRIORITY),
			mReactionSystem(nullptr),
			mUpdateInterval(DEFAULT_UPDATE_INTERVAL,DEFAULT_UPDATE_INTERVAL_JITTER)
		{
		}

		inline action::Priority ReactionComponent::getMinPriority() const
		{
			return mMinPriority;
		}

		inline void ReactionComponent::setMinPriority(action::Priority minPriority)
		{
			assignAndPromoteChange(mMinPriority,minPriority,MIN_PRIORITY_PROPERTY_ID);
		}


		inline const AssetProxy& ReactionComponent::getReactionRulesAsset() const
		{
			return mReactionRulesAsset;
		}

		inline const ReactionRulesTable& ReactionComponent::getReactions() const
		{
			return mReactions;
		}

		inline void ReactionComponent::pushEvent(const PerceptionEvent& perceptionEvent)
		{
			if (isActive())
				mPerceptionEventQueue.push(perceptionEvent);
		}

		inline PerceptionEventQueue& ReactionComponent::getPerceptionEventQueue()
		{
			return mPerceptionEventQueue;
		}

		inline UnsignedFloat ReactionComponent::getUpdateInterval() const
		{
			return mUpdateInterval.getInterval();
		}

		inline void ReactionComponent::setUpdateInterval(UnsignedFloat updateInterval)
		{
			if (mUpdateInterval.getInterval()!=updateInterval)
			{
				mUpdateInterval.setInterval(updateInterval);
				promotePropertyChange(UPDATE_INTERVAL_PROPERTY_ID);
			}
		}

		inline UnsignedFloat ReactionComponent::getUpdateIntervalJitter() const
		{
			return mUpdateInterval.getIntervalJitter();
		}

		inline void ReactionComponent::setUpdateIntervalJitter(UnsignedFloat updateIntervalJitter)
		{
			if (mUpdateInterval.getIntervalJitter()!=updateIntervalJitter)
			{
				mUpdateInterval.setIntervalJitter(updateIntervalJitter);
				promotePropertyChange(UPDATE_INTERVAL_JITTER_PROPERTY_ID);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
