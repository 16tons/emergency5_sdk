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
		inline AssetPackage& AssetImport::getAssetPackage() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mAssetPackage, "The asset package instance is invalid", QSF_REACT_THROW);
			return *mAssetPackage;
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline AssetImport::AssetImport(AssetPackage& assetPackage) :
			mAssetPackage(&assetPackage)
		{
			// Nothing to do in here
		}

		inline AssetImport::~AssetImport()
		{
			// Nothing to do in here
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
