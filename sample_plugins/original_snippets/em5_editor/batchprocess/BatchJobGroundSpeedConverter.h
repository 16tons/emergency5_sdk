// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor/batchprocess/BatchJob.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Batch job for ground speed conversion
		*
		*  @todo
		*    - TODO(co) Remove this batch job when possible, was only added for editing migration
		*/
		class BatchJobGroundSpeedConverter : public qsf::editor::BatchJob
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const char JOB_TEXT[];


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit BatchJobGroundSpeedConverter(qsf::editor::BatchJobManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BatchJobGroundSpeedConverter();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::BatchJobBase methods      ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getText() const override;
			virtual void getProgress(uint32& current, uint32& total) const override;
			virtual bool configure() override;
			virtual bool work() override;
			virtual void cleanup() override;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::BatchJobGroundSpeedConverter)
