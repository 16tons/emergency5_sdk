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
		*    Batch job for mesh LOD attachment updates
		*
		*  @remarks
		*    Processes all meshes which are considered to be base-meshes and can potentially have LODs. For all those
		*    meshes, the data source directory is checked for new mesh LODs which are then stitched to the base mesh.
		*
		*    Additionally, LOD distance settings are enforced according to our convention
		*    - LOD 0: From 0 to 30 meter
		*    - LOD 1: From 30 to 45 meter
		*    - LOD 2: From 45 meter
		*    For humans
		*    - LOD 0: From 0 to 10 meter
		*    - LOD 1: From 10 to 25 meter
		*    - LOD 2: From 25 meter
		*
		*    Mesh LOD naming convention:
		*    - LOD 0: *
		*    - LOD 1: *_lod1
		*    - LOD 2: *_lod2
		*
		*  @note
		*    - LOD mesh assets have to reuse the skeleton of the base mesh. So, this batch process enforces this.
		*    - LOD mesh assets are not allowed to have own materials which are not used by the base mesh. An error will be returned in this case and the LOD will not be added.
		*/
		class QSF_EDITOR_API_EXPORT BatchJobMeshLodSewer : public BatchJob
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const char JOB_TEXT[];
			static const uint32 NUMBER_OF_LOD_DISTANCES;
			static const float HUMAN_LOD_DISTANCE[];
			static const float LOD_DISTANCE[];


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit BatchJobMeshLodSewer(BatchJobManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BatchJobMeshLodSewer();


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
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void processMeshAsset(GlobalAssetId globalAssetId);
			const float* getLodDistanceByAssetName(const std::string& assetName) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<GlobalAssetId>		 mGlobalMeshAssetIds;
			size_t							 mCurrentIndex;
			size_t							 mNumberOfEditedAssets;
			std::shared_ptr<AssetEditHelper> mAssetEditHelper;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::BatchJobMeshLodSewer)
