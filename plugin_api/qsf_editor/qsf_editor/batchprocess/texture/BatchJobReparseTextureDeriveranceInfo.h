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
	class MaterialManager;
	namespace editor
	{
		class AssetEditHelper;
		class AssetStorageManager;
		class MaterialEditorManager;
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
		*    Batch job for material recompiling
		*/
		class QSF_EDITOR_API_EXPORT BatchJobReparseTextureDeriveranceInfo : public BatchJob
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const char JOB_TEXT[];
			static const uint32 MAX_NUMBER_OF_CONCURRENT_UPLOADS;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			explicit BatchJobReparseTextureDeriveranceInfo(BatchJobManager* manager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~BatchJobReparseTextureDeriveranceInfo();


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
			AssetStorageManager&			 mAssetStorageManager;
			MaterialEditorManager&			 mMaterialEditorManager;
			MaterialManager&				 mMaterialManager;
			std::vector<GlobalAssetId>		 mGlobalTextureAssetIds;
			std::vector<GlobalAssetId>		 mGlobalMaterialAssetIds;
			size_t							 mCurrentTextureIndex;
			size_t							 mCurrentMaterialIndex;
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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::BatchJobReparseTextureDeriveranceInfo)
