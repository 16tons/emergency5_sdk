// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LinkContainer::LinkContainer() :
		mPreventRemoval(0)
	{
		// Nothing here
	}

	inline const LinkConnectionMap& LinkContainer::getLinkConnectionMap() const
	{
		return mLinkConnectionMap;
	}

	inline bool LinkContainer::canBeRemoved() const
	{
		return (mPreventRemoval == 0) && mLinkConnectionMap.empty() && mLinkProxies.empty();
	}

	inline Link* LinkContainer::getLinkById(const LinkAnchorId& anchorId) const
	{
		LinkConnectionMap::const_iterator iterator = mLinkConnectionMap.find(anchorId);
		return (iterator != mLinkConnectionMap.end()) ? iterator->second : nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
