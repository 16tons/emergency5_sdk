// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/asset/AssetSystemTypes.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Global definitions                                    ]
		//[-------------------------------------------------------]
		enum AssetTransferType
		{
			TT_DOWNLOAD_CACHED,
			TT_DOWNLOAD_SOURCE,
			TT_UPLOAD_SOURCE
		};

		struct EditorCachedAssetDownload
		{
			ProjectId projectId;
			std::string assetPackageName;
			GlobalAssetId globalAssetId;
			AssetRevision cachedAssetRevision;

			inline bool operator<(const EditorCachedAssetDownload& rhs) const
			{
				bool result = false;

				if (globalAssetId < rhs.globalAssetId)
				{
					result = true;
				}
				else if (globalAssetId == rhs.globalAssetId)
				{
					if (projectId < rhs.projectId)
					{
						result = true;
					}
					else if (projectId == rhs.projectId)
					{
						if (cachedAssetRevision < rhs.cachedAssetRevision)
						{
							result = true;
						}
						else if (cachedAssetRevision == rhs.cachedAssetRevision)
						{
							result = (assetPackageName < rhs.assetPackageName);
						}
					}
				}

				return result;
			}
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
