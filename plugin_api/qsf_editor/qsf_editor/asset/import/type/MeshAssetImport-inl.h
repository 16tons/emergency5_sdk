// Copyright (C) 2012-2018 Promotion Software GmbH


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
		inline MeshAssetImport::MeshAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup) :
			AssetImport(assetPackage),
			mOgreResourceGroup(ogreResourceGroup),
			mGlobalSkeletonAnimationAssetId(getUninitialized<GlobalAssetId>())
		{
			// Nothing to do in here
		}

		inline MeshAssetImport::~MeshAssetImport()
		{
			// Nothing to do in here
		}

		inline GlobalAssetId MeshAssetImport::getGlobalSkeletonAnimationAssetId() const
		{
			return mGlobalSkeletonAnimationAssetId;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
