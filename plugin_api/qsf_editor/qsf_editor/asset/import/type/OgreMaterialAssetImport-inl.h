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
		inline OgreMaterialAssetImport::OgreMaterialAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup, const std::string& resourceDirectory, const std::string& qsfBaseMaterial) :
			AssetImport(assetPackage),
			mOgreResourceGroup(ogreResourceGroup),
			mResourceDirectory(resourceDirectory),
			mQsfBaseMaterial(qsfBaseMaterial)
		{
			// Nothing to do in here
		}

		inline OgreMaterialAssetImport::~OgreMaterialAssetImport()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
