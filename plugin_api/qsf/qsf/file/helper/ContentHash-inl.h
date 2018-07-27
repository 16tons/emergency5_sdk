// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ContentHash::~ContentHash()
	{
		// Nothing here
	}

	inline uint64 ContentHash::getHash() const
	{
		return mHash;
	}

	inline ContentHash::operator uint64() const
	{
		return mHash;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
