// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline LocalAssetIssue::LocalAssetIssue() :
			AssetIssue()
		{
			// Nothing to do in here
		}

		inline LocalAssetIssue::LocalAssetIssue(const std::string& description) :
			AssetIssue(description)
		{
			// Nothing to do in here
		}

		inline LocalAssetIssue::~LocalAssetIssue()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::LocalAssetIssue methods ]
		//[-------------------------------------------------------]
		inline bool LocalAssetIssue::applyFix(uint32 index, std::shared_ptr<AssetEditHelper> assetEditHelper)
		{
			// Failed to apply the fix
			return false;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
