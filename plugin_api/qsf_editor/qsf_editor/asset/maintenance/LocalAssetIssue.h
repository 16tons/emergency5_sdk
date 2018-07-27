// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf_editor_base/asset/maintenance/AssetIssue.h>

#include <qsf/asset/AssetSystemTypes.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class AssetIssueFixContext;
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
		*    Asset issue base class
		*/
		class QSF_EDITOR_API_EXPORT LocalAssetIssue : public base::AssetIssue
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class AssetIssueFixContext; // Only AssetIssueFixContext may call applyFix(...)


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			inline LocalAssetIssue();

			inline explicit LocalAssetIssue(const std::string& description);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~LocalAssetIssue();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::LocalAssetIssue methods ]
		//[-------------------------------------------------------]
		protected:
			/**
			*  @brief
			*    Tries to apply the automatic fix
			*
			*  @param[in] index
			*    Index of the fix to apply
			*
			*  @return
			*    "true" if issue was successfully fixed, "false" otherwise
			*
			*  @note
			*    - The default implementation is not able to fix the issue and hence always returns false
			*/
			inline virtual bool applyFix(uint32 index, std::shared_ptr<AssetEditHelper> assetEditHelper);


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/asset/maintenance/LocalAssetIssue-inl.h"
