// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ShaderProperties::ShaderProperties()
	{
		// Nothing here
	}

	inline ShaderProperties::~ShaderProperties()
	{
		// Nothing here
	}

	inline const ShaderProperties::SortedPropertyVector& ShaderProperties::getSortedPropertyVector() const
	{
		return mSortedPropertyVector;
	}

	inline ShaderProperties::SortedPropertyVector& ShaderProperties::getSortedPropertyVector()
	{
		return mSortedPropertyVector;
	}

	inline void ShaderProperties::clear()
	{
		mSortedPropertyVector.clear();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
