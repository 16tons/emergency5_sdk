// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CommandContext::CommandContext() :
		mPlayer(nullptr),
		mIsAutomatism(false),
		mEvaluationReason(EVALUATIONREASON_UNKNOWN)
	{
		// Nothing here
	}

	inline CommandContext::~CommandContext()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
