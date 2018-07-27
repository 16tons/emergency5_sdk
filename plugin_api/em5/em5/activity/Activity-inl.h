// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Activity::Activity(uint32 id) :
		mId(id),
		mLoadVersion(0)
	{
		// Nothing here
	}

	inline Activity::~Activity()
	{
		clear();
	}

	inline uint32 Activity::getId() const
	{
		return mId;
	}

	inline const qsf::AssetProxy& Activity::getSourceAssetProxy() const
	{
		return mSourceAssetProxy;
	}

	inline uint32 Activity::getInitialStateId() const
	{
		return mInitialState.getHash();
	}

	inline const Activity::State* Activity::getStateById(uint32 id) const
	{
		StateMap::const_iterator iterator = mStates.find(id);
		return (iterator != mStates.end()) ? &iterator->second : nullptr;
	}

	inline uint32 Activity::getLoadVersion() const
	{
		return mLoadVersion;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
