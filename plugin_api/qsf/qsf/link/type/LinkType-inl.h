// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LinkType::~LinkType()
	{
		// Nothing here
	}

	inline uint32 LinkType::getId() const
	{
		return mId;
	}

	inline uint32 LinkType::getBaseId() const
	{
		return mBaseId;
	}

	inline bool LinkType::supportsMultipleConnections(link::Direction linkDirection) const
	{
		return mMultipleConnections[linkDirection];
	}

	inline const LinkType::TypeIdArray& LinkType::getInheritedTypeIds() const
	{
		return mInheritedIds;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
