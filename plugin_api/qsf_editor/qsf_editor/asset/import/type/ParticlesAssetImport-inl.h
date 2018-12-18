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
		inline ParticlesAssetImport::ParticlesAssetImport(AssetPackage& assetPackage, const std::string& ogreResourceGroup) :
			AssetImport(assetPackage),
			mOgreResourceGroup(ogreResourceGroup)
		{
			// Nothing to do in here
		}

		inline ParticlesAssetImport::~ParticlesAssetImport()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
