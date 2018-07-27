// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/batchprocess/BatchJobManager.h"

#include <qsf/plugin/pluginable/Pluginable.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Batch job base class
		*/
		class QSF_EDITOR_API_EXPORT BatchJob : public Pluginable<BatchJobManager>
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit BatchJob(BatchJobManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BatchJob();

			uint32 getId() const;

			bool isError() const;

			const std::string& getError() const;
			void setError(const std::string& errorMessage);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::BatchJobBase methods      ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getText() const = 0;

			virtual void getProgress(uint32& current, uint32& total) const = 0;

			/**
			*  @brief
			*    Called when the job is being prepared for execution
			*
			*  @return
			*    "false" to indicate an error and cancel execution
			*
			*  @note
			*    - Open your config-dialogs here in case you need user input
			*/
			virtual bool configure() = 0;

			/**
			*  @brief
			*    Called periodically while the job is running
			*
			*  @return
			*    "true" to be called again, "false" otherwise
			*/
			virtual bool work() = 0;

			/**
			*  @brief
			*    Called once when execution of the job is being stopped
			*
			*  @return
			*    "false" to indicate an error and cancel execution
			*
			*  @note
			*    - This method will be called after the job is finished, got canceled or encountered an error.
			*/
			virtual void cleanup() = 0;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		public:
			std::string mErrorMessage;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::BatchJob)
