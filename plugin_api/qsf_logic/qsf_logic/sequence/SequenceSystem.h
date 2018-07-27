// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"

#include <qsf/base/System.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{
		class SequenceManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sequence system class
		*
		*  @note
		*    - A sequence system holds a sequence manager
		*
		*  @todo
		*    - TODO(co) As far as I can tell at the moment, there's no need for a sequence manager because the sequence system could also do this job
		*/
		class QSF_LOGIC_API_EXPORT SequenceSystem : public System
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline SequenceSystem();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~SequenceSystem();

			//[-------------------------------------------------------]
			//[ Manager                                               ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return the main sequence manager instance
			*
			*  @return
			*    The main sequence manager instance, do not destroy the instance
			*/
			inline SequenceManager& getManager() const;


		//[-------------------------------------------------------]
		//[ Public virtual qsf::System methods                    ]
		//[-------------------------------------------------------]
		public:
			inline virtual const char* getName() const override;

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Startup the sequence system
			*
			*  @return
			*    "true" if all went fine, else "false"
			*
			*  @remarks
			*    The implementation does the following tasks:
			*    - Create the main sequence manager instance
			*/
			virtual bool onStartup(bool serverMode) override;

			/**
			*  @brief
			*    Shutdown the sequence system
			*
			*  @note
			*    - This method is only called in case "onStartup()" returned successfully
			*
			*  @remarks
			*    The implementation does the following tasks:
			*    - Destroy the main sequence manager instance
			*/
			virtual void onShutdown() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			SequenceManager* mMainManager;	///< Main sequence manager instance, can be a null pointer, we're responsible for destroying the instance in case we no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/SequenceSystem-inl.h"
