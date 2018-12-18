// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 Objective::getId() const
	{
		return mUniqueId;
	}

	inline uint32 Objective::getTypeId() const
	{
		return mTypeId;
	}

	inline Objective::ObjectiveType Objective::getObjectiveType() const
	{
		return mObjectiveType;
	}

	inline void Objective::setObjectiveType(ObjectiveType objectiveType)
	{
		mObjectiveType = objectiveType;
	}

	inline uint32 Objective::getFreeplayEventId()
	{
		return mFreeplayEventId;
	}

	inline const std::string& Objective::getText() const
	{
		return mText;
	}

	inline void Objective::setText(const std::string& text)
	{
		mText = text;
	}

	inline uint32 Objective::getCurrentNumber() const
	{
		return mCurrentNumber;
	}

	inline uint32 Objective::getNeededNumber() const
	{
		return mNeededNumber;
	}

	inline const Objective::EntityIdSet& Objective::getCurrentEntityIdSet() const
	{
		return mCurrentEntities;
	}

	inline const Objective::EntityIdSet& Objective::getNeededEntityIdSet() const
	{
		return mNeededEntities;
	}

	inline bool Objective::checkAccomplished() const
	{
		if (mObjectiveType == OBJECTIVETYPE_OPTIONAL_FAILED)
			return false;

		if (mNeededNumber == 0)
		{
			// Special case: Needed number is 0
			//  -> For fail conditions, this counts as "not accomplished" (i.e. not failed)
			//  -> Otherwise, this counts as "accomplished" (because nothing left to do)
			return (mObjectiveType != OBJECTIVETYPE_FAILCONDITION);
		}

		return (mCurrentNumber >= mNeededNumber);
	}

	inline const Objective::PlayerCountMap& Objective::getPlayerCountMap() const
	{
		return mPlayerCounter;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
