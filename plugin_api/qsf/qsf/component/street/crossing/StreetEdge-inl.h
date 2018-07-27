// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline StreetEdge::StreetEdge() :
		mBeginNode(getUninitialized<uint32>()),
		mEndNode(getUninitialized<uint32>()),
		mLaneType(0),
		mTag(0)
	{
		// Nothing to do in here
	}

	inline StreetEdge::StreetEdge(uint32 beginNode, uint32 endNode, uint32 laneType, uint32 tag) :
		mBeginNode(beginNode),
		mEndNode(endNode),
		mLaneType(laneType),
		mTag(tag)
	{
		// Nothing to do in here
	}

	inline StreetEdge::StreetEdge(const StreetEdge& source) :
		mBeginNode(source.mBeginNode),
		mEndNode(source.mEndNode),
		mLaneType(source.mLaneType),
		mTag(source.mTag)
	{
		// Nothing to do in here
	}

	inline bool StreetEdge::isValid() const
	{
		// TODO(mk) Add (mBeginNode != mEndNode) check
		return (isInitialized(mBeginNode) && isInitialized(mEndNode));
	}

	inline uint32 StreetEdge::getBeginNode() const
	{
		return mBeginNode;
	}

	inline void StreetEdge::setBeginNode(uint32 beginNode)
	{
		mBeginNode = beginNode;
	}

	inline uint32 StreetEdge::getEndNode() const
	{
		return mEndNode;
	}

	inline void StreetEdge::setEndNode(uint32 endNode)
	{
		mEndNode = endNode;
	}

	inline uint32 StreetEdge::getLaneType() const
	{
		return mLaneType;
	}

	inline void StreetEdge::setLaneType(uint32 laneType)
	{
		mLaneType = laneType;
	}

	inline uint32 StreetEdge::getTag() const
	{
		return mTag;
	}

	inline void StreetEdge::setTag(uint32 tag)
	{
		mTag = tag;
	}

	//[-------------------------------------------------------]
	//[ Assignment operators                                  ]
	//[-------------------------------------------------------]
	inline StreetEdge& StreetEdge::operator =(const StreetEdge& source)
	{
		mBeginNode = source.mBeginNode;
		mEndNode = source.mEndNode;
		mLaneType = source.mLaneType;
		mTag = source.mTag;
		return *this;
	}

	inline bool StreetEdge::operator ==(const StreetEdge& other) const
	{
		return (mBeginNode == other.mBeginNode && mEndNode == other.mEndNode && mLaneType == other.mLaneType && mTag == other.mTag);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
}
