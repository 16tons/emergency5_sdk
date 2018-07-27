// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SegmentDebugDrawRequest::SegmentDebugDrawRequest(const Segment& segment, const Color4& color) :
		mSegment(segment),
		mColor(color)
	{
		// Nothing to do in here
	}

	inline SegmentDebugDrawRequest::~SegmentDebugDrawRequest()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	inline SegmentDebugDrawRequest::SegmentDebugDrawRequest() :
		mSegment(Segment::fromUpDirection())
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
