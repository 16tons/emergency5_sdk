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
		inline UnknownAssetImport::UnknownAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup, const std::string& resourceDirectory) :
			AssetImport(assetPackage),
			mOgreResourceGroup(ogreResourceGroup),
			mResourceDirectory(resourceDirectory)
		{
			// Nothing to do in here
		}

		inline UnknownAssetImport::~UnknownAssetImport()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
