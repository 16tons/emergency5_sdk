// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RightsManager::~RightsManager()
	{
		// Nothing here
	}

	inline bool RightsManager::hasRight(const qsf::StringHash& id) const
	{
		return (mRightIds.find(id) != mRightIds.cend());
	}

	inline const RightsManager::RightIds& RightsManager::getRightIds() const
	{
		return mRightIds;
	}

	inline void RightsManager::setRightIds(const RightIds& rightIds)
	{
		mRightIds = rightIds;
		RightsChange(*this);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
