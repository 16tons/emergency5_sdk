// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DynamicMusicCompositorComponent::DynamicMusicCompositorComponent(Prototype* prototype) :
		Component(prototype),
		mDefaultMusicFadeTime(Time::fromSeconds(1.0f)),
		mMaxMusicVolume(1.0f),
		mCurrentLevel(-1),
		mNextLevel(-1),
		mCurrentTrackId(-1),
		mNextTrackId(-1),
		mCurrentLoop(0),
		MusicFiles(*this)
	{
		// Nothing to do in here
	}

	inline DynamicMusicCompositorComponent::~DynamicMusicCompositorComponent()
	{
		// Nothing to do here
	}

	inline const std::vector<std::string>& DynamicMusicCompositorComponent::getMusicFiles() const
	{
		return mMusicFilenames;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t DynamicMusicCompositorComponent::MusicFilesInterface::size() const
	{
		return getOwner().getMusicFiles().size();
	}

	inline const std::string& DynamicMusicCompositorComponent::MusicFilesInterface::get(std::size_t index) const
	{
		return getOwner().getMusicFiles().at(index);
	}

	inline void DynamicMusicCompositorComponent::MusicFilesInterface::set(std::size_t index, const std::string& value)
	{
		getOwner().setNode(index, value);
	}

	inline void DynamicMusicCompositorComponent::MusicFilesInterface::insert(std::size_t before, const std::string& value)
	{
		getOwner().insertNode(before, value);
	}

	inline void DynamicMusicCompositorComponent::MusicFilesInterface::remove(std::size_t index)
	{
		getOwner().removeNode(index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
