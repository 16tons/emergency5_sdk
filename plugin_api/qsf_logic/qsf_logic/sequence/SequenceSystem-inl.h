// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline SequenceSystem::SequenceSystem() :
			mMainManager(nullptr)
		{
			// Nothing to do in here
		}

		inline SequenceSystem::~SequenceSystem()
		{
			// Nothing to do in here
			QSF_ASSERT(nullptr == mMainManager, "The sequence system still contains a manager which should have been destroyed on shutdown", QSF_REACT_NONE);
		}

		inline SequenceManager& SequenceSystem::getManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mMainManager, "The main sequence manager instance is invalid", QSF_REACT_THROW);
			return *mMainManager;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		inline const char* SequenceSystem::getName() const
		{
			return "Sequence system";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
