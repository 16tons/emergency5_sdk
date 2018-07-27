// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/batchprocess/BatchJob.h"

#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class AssetEditHelper;
	}
}


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
		*    Batch job for reassign LOD mesh metadata recompiling
		*/
		class QSF_EDITOR_API_EXPORT BatchJobReassignLodMeshMetadata : public BatchJob
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
			explicit BatchJobReassignLodMeshMetadata(BatchJobManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BatchJobReassignLodMeshMetadata();


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
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<GlobalAssetId>				  mGlobalMeshAssetIds;
			boost::container::flat_set<GlobalAssetId> mGlobalLodMeshAssetIds;
			size_t									  mCurrentIndex;
			size_t									  mMeshesWithLod;
			size_t									  mMeshesWithoutLod;
			std::shared_ptr<AssetEditHelper>		  mAssetEditHelper;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::BatchJobReassignLodMeshMetadata)
