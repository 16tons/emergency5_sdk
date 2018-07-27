// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MaterialProperties::MaterialProperties()
	{
		// Nothing here
	}

	inline MaterialProperties::~MaterialProperties()
	{
		// Nothing here
	}

	inline const MaterialProperties::SortedPropertyVector& MaterialProperties::getSortedPropertyVector() const
	{
		return mSortedPropertyVector;
	}

	inline void MaterialProperties::removeAllProperties()
	{
		mSortedPropertyVector.clear();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
