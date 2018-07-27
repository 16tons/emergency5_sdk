// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_compositing/Export.h"

#include <qsf/base/System.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{
		class ScreenshotManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace compositing
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Compositor system
		*
		*  @note
		*    - "Compositor Manager" in OGRE terminology (don't dare to rename the QSF class because the big picture terminology must be conistent inside QSF)
		*/
		class QSF_COMPOSITING_API_EXPORT CompositorSystem : public System
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			CompositorSystem();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~CompositorSystem();

			//[-------------------------------------------------------]
			//[ Manager                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the screenshot manager instance
			*
			*  @return
			*    The screenshot manager instance, do no destroy the returned instance
			*/
			inline ScreenshotManager& getScreenshotManager() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		public:
			inline virtual const char* getName() const override;

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup(bool serverMode) override;
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ScreenshotManager* mScreenshotManager;	///< Screenshot manager instance, always valid, we're responsible for destroying the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // compositing
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_compositing/compositor/CompositorSystem-inl.h"
