// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/maintenance/LocalAssetIssue.h"


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
		*    Missing local file asset issue class
		*/
		class MissingLocalFileIssue : public LocalAssetIssue
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 ISSUE_ID;	///< String hash of "qsf::editor::MissingLocalFileIssue"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			MissingLocalFileIssue(GlobalAssetId globalAssetId, const std::string& filename);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MissingLocalFileIssue();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetIssue methods  ]
		//[-------------------------------------------------------]
		public:
			virtual uint32 getIssueId() const override;
			virtual uint32 countFixes() const override;
			virtual bool isFixApplicable(uint32 index) const override;
			virtual const std::string& getFixDescription(uint32 index) const override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::LocalAssetIssue methods ]
		//[-------------------------------------------------------]
		protected:
			virtual bool applyFix(uint32 index, std::shared_ptr<AssetEditHelper> assetEditHelper) override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
