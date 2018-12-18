// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor/batchprocess/BatchJob.h>

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
namespace em5
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Batch job for migrating skeleton coordinate system
		*
		*  @note
		*    - Only exists to update assets which had been imported before (exported from Autodesk Maya, now exported from Autodesk 3ds Max)
		*/
		class BatchJobMigrateSkeletonCoordinateSystem : public qsf::editor::BatchJob
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const char		 JOB_TEXT[];
			static const std::string OLD_SKELETON_TAG;
			static const std::string NEW_SKELETON_TAG;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit BatchJobMigrateSkeletonCoordinateSystem(qsf::editor::BatchJobManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BatchJobMigrateSkeletonCoordinateSystem();

			inline void setInverse(bool inverse) { mInverse = inverse; }


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
			std::vector<qsf::GlobalAssetId>				  mGlobalSkeletonAssetIds;
			size_t										  mCurrentIndex;
			size_t										  mNumberOfEditedAssets;
			std::shared_ptr<qsf::editor::AssetEditHelper> mAssetEditHelper;
			bool										  mInverse;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


		// TODO(fw): It would be nicer to have batch job configurable -- or is this possible maybe already?
		class BatchJobMigrateSkeletonCoordinateSystemInverse : public BatchJobMigrateSkeletonCoordinateSystem
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
			explicit BatchJobMigrateSkeletonCoordinateSystemInverse(qsf::editor::BatchJobManager* manager);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::BatchJobBase methods      ]
		//[-------------------------------------------------------]
		public:
			virtual std::string getText() const override;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::BatchJobMigrateSkeletonCoordinateSystem)
QSF_CAMP_TYPE_NONCOPYABLE(em5::editor::BatchJobMigrateSkeletonCoordinateSystemInverse)
