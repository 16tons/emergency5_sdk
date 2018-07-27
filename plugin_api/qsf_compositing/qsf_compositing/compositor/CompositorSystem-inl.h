// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline ScreenshotManager& CompositorSystem::getScreenshotManager() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mScreenshotManager, "The screenshot manager instance is invalid", QSF_REACT_THROW);
			return *mScreenshotManager;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		inline const char* CompositorSystem::getName() const
		{
			return "Compositor system";
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf
