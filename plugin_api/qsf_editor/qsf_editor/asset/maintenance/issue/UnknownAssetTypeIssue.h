// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_editor_base/asset/maintenance/AssetIssue.h>


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
		*    Unknown asset type issue class
		*/
		class UnknownAssetTypeIssue : public base::AssetIssue
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 ISSUE_ID;	///< String hash of "qsf::editor::UnknownAssetTypeIssue"


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			explicit UnknownAssetTypeIssue(GlobalAssetId globalAssetId);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~UnknownAssetTypeIssue();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::base::AssetIssue methods  ]
		//[-------------------------------------------------------]
		public:
			virtual uint32 getIssueId() const override;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
