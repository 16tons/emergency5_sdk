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
	inline Link::Link() :
		mLinkType(nullptr),
		mSource(nullptr),
		mTarget(nullptr)
	{
		// Nothing here
	}

	inline Link::~Link()
	{
		// Nothing here
	}

	inline const LinkType& Link::getLinkType() const
	{
		QSF_CHECK(nullptr != mLinkType, "Link type is a null pointer", QSF_REACT_THROW);
		return *mLinkType;
	}

	inline uint32 Link::getLinkTypeId() const
	{
		return getLinkType().getId();
	}

	inline LinkAnchor& Link::getSourceAnchor() const
	{
		QSF_CHECK(nullptr != mSource, "Source link anchor is invalid", QSF_REACT_THROW);
		return *mSource;
	}

	inline LinkAnchor& Link::getTargetAnchor() const
	{
		QSF_CHECK(nullptr != mTarget, "Target link anchor is invalid", QSF_REACT_THROW);
		return *mTarget;
	}

	inline LinkAnchor& Link::getAnchor(link::Direction linkDirection) const
	{
		return (linkDirection == link::FORWARD) ? getSourceAnchor() : getTargetAnchor();
	}

	inline LinkAnchor& Link::getRemoteAnchor(link::Direction linkDirection) const
	{
		return (linkDirection == link::BACKWARD) ? getSourceAnchor() : getTargetAnchor();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Link methods                   ]
	//[-------------------------------------------------------]
	inline void Link::onDisconnect()
	{
		// Default implementation is empty
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
