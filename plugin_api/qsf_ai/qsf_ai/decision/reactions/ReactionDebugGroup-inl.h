// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		inline ReactionDebugGroup::ReactionDebugGroup(DebugGroupManager* dgm) :
			DebugGroup(dgm),
			mShowForAllEntities(DEFAULT_SHOW_FOR_ALL_ENTITIES),
			mShowMessages(DEFAULT_SHOW_MESSAGES),
			mFireTestEventAInterval(DEFAULT_TEST_EVENT_A_INTERVAL),
			mFireTestEventBDelay(DEFAULT_TEST_EVENT_B_DELAY),
			mTestEventState(FIRE_A),
			mTimeToTestEvent(DEFAULT_TEST_EVENT_A_INTERVAL)
		{
		}

		inline bool ReactionDebugGroup::getShowForAllEntities() const
		{
			return mShowForAllEntities;
		}

		inline void ReactionDebugGroup::setShowForAllEntities(bool showForAllEntities)
		{
			assignAndPromoteChange(mShowForAllEntities,showForAllEntities,SHOW_FOR_ALL_ENTITIES_PROPERTY_ID);
		}

		inline bool ReactionDebugGroup::getShowMessages() const
		{
			return mShowMessages;
		}

		inline void ReactionDebugGroup::setShowMessages(bool showMessages)
		{
			assignAndPromoteChange(mShowMessages,showMessages,SHOW_MESSAGES_PROPERTY_ID);
		}

		inline const std::string& ReactionDebugGroup::getGlobalTestEventA() const
		{
			return mGlobalTestEventA.getName();
		}

		inline void ReactionDebugGroup::setGlobalTestEventA(const std::string& globalTestEvent)
		{
			assignAndPromoteChange(mGlobalTestEventA,NamedIdentifier(globalTestEvent),GLOBAL_TEST_EVENT_A_PROPERTY_ID);
		}

		inline const std::string& ReactionDebugGroup::getGlobalTestEventB() const
		{
			return mGlobalTestEventB.getName();
		}

		inline void ReactionDebugGroup::setGlobalTestEventB(const std::string& globalTestEvent)
		{
			assignAndPromoteChange(mGlobalTestEventB,NamedIdentifier(globalTestEvent),GLOBAL_TEST_EVENT_B_PROPERTY_ID);
		}

		inline UnsignedFloat ReactionDebugGroup::getFireTestEventAInterval() const
		{
			return mFireTestEventAInterval;
		}

		inline void ReactionDebugGroup::setFireTestEventAInterval(UnsignedFloat interval)
		{
			assignAndPromoteChange(mFireTestEventAInterval,interval,TEST_EVENT_A_INTERVAL_PROPERTY_ID);
		}

		inline UnsignedFloat ReactionDebugGroup::getFireTestEventBDelay() const
		{
			return mFireTestEventBDelay;
		}

		inline void ReactionDebugGroup::setFireTestEventBDelay(UnsignedFloat interval)
		{
			assignAndPromoteChange(mFireTestEventBDelay,interval,TEST_EVENT_B_DELAY_PROPERTY_ID);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
