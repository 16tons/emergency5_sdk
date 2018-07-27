// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline AudioSpecsGroup::AudioSpecsGroup(SpecsGroupManager* specsGroupManager) :
		SpecsGroup(specsGroupManager),
		mMaxMusicVolume(1.f)
	{
		// Nothing to do in here
	}

	inline AudioSpecsGroup::~AudioSpecsGroup()
	{
		// Nothing to do in here
	}

	inline float AudioSpecsGroup::getMaxMusicVolume() const
	{
		return mMaxMusicVolume;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
