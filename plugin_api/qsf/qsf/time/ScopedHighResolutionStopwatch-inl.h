// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ScopedHighResolutionStopwatch::ScopedHighResolutionStopwatch(Time& outputTime) :
		HighResolutionStopwatch(true),
		mOutputTime(outputTime)
	{
		// Nothing to do in here
	}

	inline ScopedHighResolutionStopwatch::~ScopedHighResolutionStopwatch()
	{
		mOutputTime += getElapsed();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
