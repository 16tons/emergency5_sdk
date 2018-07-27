// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Asset::~Asset()
	{
		// Nothing to do in here
	}

	inline Asset& Asset::operator =(const Asset& source)
	{
		// Call parent implementation
		*static_cast<BaseAsset*>(this) = static_cast<const BaseAsset&>(source);

		// Just copy over the content, no need for additional work to be done
		mPriority = source.mPriority;

		return *this;
	}

	inline Asset& Asset::copyPropertiesFrom(const Asset& source)
	{
		// Backup our global asset id
		const GlobalAssetId globalAssetId = mGlobalAssetId;

		// Call the copy operator
		operator=(source);

		// Now fix the global asset id
		mGlobalAssetId = globalAssetId;

		return *this;
	}

	inline float Asset::getPriority() const
	{
		return mPriority;
	}

	inline void Asset::setPriority(float priority)
	{
		mPriority = priority;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
