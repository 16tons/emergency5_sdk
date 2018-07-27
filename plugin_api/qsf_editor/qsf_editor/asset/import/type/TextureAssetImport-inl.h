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
		inline TextureAssetImport::TextureAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup, const std::string& resourceDirectory, bool compileTexture) :
			AssetImport(assetPackage),
			mOgreResourceGroup(ogreResourceGroup),
			mResourceDirectory(resourceDirectory),
			mCompileTexture(compileTexture)
		{
			// Nothing to do in here
		}

		inline TextureAssetImport::~TextureAssetImport()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
