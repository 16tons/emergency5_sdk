// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{


			//[-------------------------------------------------------]
			//[ Public methods                                        ]
			//[-------------------------------------------------------]
			inline AssetIssue::~AssetIssue()
			{
				// Nothing to do in here
			}

			inline GlobalAssetId AssetIssue::getGlobalAssetId() const
			{
				return mGlobalAssetId;
			}

			inline AssetPackage* AssetIssue::getAssetPackage() const
			{
				return mAssetPackage;
			}

			inline void AssetIssue::setAssetPackage(AssetPackage* assetPackage)
			{
				mAssetPackage = assetPackage;
			}

			inline const std::string& AssetIssue::getDescription() const
			{
				return mDescription;
			}

			inline void AssetIssue::setDescription(const std::string& description)
			{
				mDescription = description;
			}

			inline const std::string& AssetIssue::getFilename() const
			{
				return mFilename;
			}

			inline void AssetIssue::setFilename(const std::string& filename)
			{
				mFilename = filename;
			}

			inline AssetIssue::Severity AssetIssue::getSeverity() const
			{
				return mSeverity;
			}

			inline void AssetIssue::setSeverity(AssetIssue::Severity severity)
			{
				mSeverity = severity;
			}


			//[-------------------------------------------------------]
			//[ Public virtual qsf::editor::base::AssetIssue methods  ]
			//[-------------------------------------------------------]
			inline uint32 AssetIssue::countFixes() const
			{
				// No fix available
				return 0;
			}

			inline bool AssetIssue::isFixApplicable(uint32 index) const
			{
				// No fix available
				return false;
			}

			inline const std::string& AssetIssue::getFixDescription(uint32 index) const
			{
				static const std::string emptyString;

				// No fix available
				return emptyString;
			}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // base
	} // editor
} // qsf
