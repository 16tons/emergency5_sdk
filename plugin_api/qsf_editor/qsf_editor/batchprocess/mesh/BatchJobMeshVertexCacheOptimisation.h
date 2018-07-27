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
namespace Ogre
{
	namespace v1
	{
		class Mesh;
	}
}
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
		*    Batch job for mesh vertex cache optimisation
		*
		*  @note
		*    - Using Tom Forsyth's linear-speed vertex cache optimisation algorithm - http://home.comcast.net/%7Etom_forsyth/papers/fast_vert_cache_opt.html
		*    - Using https://github.com/vivkin/forsyth - implementation by Martin Storsjo in header file single-file library
		*    - Mesh vertex cache optimisation is automatically done when importing new meshes, this batch process only exists to update already imported meshes
		*    - Only brings any benefit if vertices are shared between triangles
		*/
		class QSF_EDITOR_API_EXPORT BatchJobMeshVertexCacheOptimisation : public BatchJob
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const char JOB_TEXT[];


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Optimize OGRE mesh
			*
			*  @param[in] ogreMesh
			*    OGRE mesh to optimize
			*/
			static void optimizeOgreMesh(Ogre::v1::Mesh& ogreMesh);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit BatchJobMeshVertexCacheOptimisation(BatchJobManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BatchJobMeshVertexCacheOptimisation();


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::BatchJobMeshVertexCacheOptimisation)
