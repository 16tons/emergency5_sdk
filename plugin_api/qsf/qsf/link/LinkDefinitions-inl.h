// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Definitions (qsf namespace)                           ]
	//[-------------------------------------------------------]
	inline LinkAnchorId::LinkAnchorId(uint32 contextId, uint64 localId) :
		mContextId(contextId),
		mLocalId(localId)
	{
		// Nothing here
	}

	inline bool LinkAnchorId::operator<(const LinkAnchorId& other) const
	{
		return (mContextId == other.mContextId) ? (mLocalId < other.mLocalId) : (mContextId > other.mContextId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
