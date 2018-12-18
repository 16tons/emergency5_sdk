// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LinkAnchor::LinkAnchor(const LinkAnchorId& anchorId) :
		mId(anchorId)
	{
		// Nothing here
	}

	inline LinkAnchor::~LinkAnchor()
	{
		for (int i = 0; i < 2; ++i)
		{
			// Sanity check: all links should have been removed before
			QSF_CHECK(mLinks[i].empty(), "Link anchor is not empty on destruction, this should not happen", QSF_REACT_NONE);

			// Clear the link container map
			// -> This destroys all remaining link containers and invalidates the registered link proxy, if there is one
			// -> It is not necessary to explicitly call clear(), but also it does not harm (and I can better comment it)
			mLinkContainers[i].clear();
		}
	}

	inline const LinkAnchorId& LinkAnchor::getId() const
	{
		return mId;
	}

	inline const std::vector<Link*>& LinkAnchor::getLinks(link::Direction linkDirection) const
	{
		return mLinks[linkDirection];
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::LinkAnchor methods             ]
	//[-------------------------------------------------------]
	inline void LinkAnchor::onStructuralChange()
	{
		// Default implementation is empty
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
