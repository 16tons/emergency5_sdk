// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


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
		inline AssetPackageImportHelper::AssetPackageImportHelper(AssetPackage& assetPackage) :
			mAssetPackage(&assetPackage)
		{
			// Nothing to do in here
		}

		inline AssetPackageImportHelper::~AssetPackageImportHelper()
		{
			// Nothing to do in here
		}

		inline AssetPackage& AssetPackageImportHelper::getAssetPackageSafe() const
		{
			// It might not be safe to use this method in certain situations
			QSF_CHECK(nullptr != mAssetPackage, "The asset package instance is invalid", QSF_REACT_THROW);
			return *mAssetPackage;
		}

		inline AssetPackage* AssetPackageImportHelper::getAssetPackage() const
		{
			return mAssetPackage;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
