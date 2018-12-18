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
		inline OgreMaxSceneAssetImport::OgreMaxSceneAssetImport(PrototypeManager& prototypeManager, AssetPackage& assetPackage, const std::string& ogreResourceGroup) :
			AssetImport(assetPackage),
			mPrototypeManager(prototypeManager),
			mOgreResourceGroup(ogreResourceGroup)
		{
			// Nothing to do in here
		}

		inline OgreMaxSceneAssetImport::~OgreMaxSceneAssetImport()
		{
			// Nothing to do in here
		}

		inline const OgreMaxSceneAssetImport::DirectGlobalAssetIdDependencies& OgreMaxSceneAssetImport::getDirectGlobalAssetIdDependencies() const
		{
			return mDirectGlobalAssetIdDependencies;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
