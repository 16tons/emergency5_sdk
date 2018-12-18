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
		const std::string& EditedMaterialInstance::getName() const
		{
			return mName;
		}

		const std::string& EditedMaterialInstance::getBaseMaterialName() const
		{
			return mBaseMaterialName;
		}

		bool EditedMaterialInstance::isValid() const
		{
			return isInitialized(mGlobalAssetId);
		}

		bool EditedMaterialInstance::isOpen() const
		{
			return mIsOpenForEdit;
		}

		bool EditedMaterialInstance::isBad() const
		{
			return mHasOpenForEditFailed;
		}

		bool EditedMaterialInstance::isDeleted() const
		{
			return mIsDeleted;
		}

		bool EditedMaterialInstance::needsTextureCompilation() const
		{
			return mNeedsTextureCompilation;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
