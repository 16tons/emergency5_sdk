// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline CommandContext::CommandContext() :
			mCaller(nullptr),
			mTargetEntity(nullptr),
			mTargetPositionSealed(false),
			mUserData(qsf::getUninitialized<uint64>()),
			mAllowSelfExecution(false),
			mCurrentCallerIndex(0)
		{
			// Nothing here
		}

		inline CommandContext::~CommandContext()
		{
			// Nothing here
		}

		inline bool CommandContext::hasTargetPosition() const
		{
			return mTargetPosition.is_initialized();
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
