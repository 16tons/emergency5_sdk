// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool WarblerFeedWindow::isEnabled() const
	{
		return mEnabled;
	}

	inline WarblerFeedWindow::Flags WarblerFeedWindow::getFlags() const
	{
		return mFlags;
	}

	inline void WarblerFeedWindow::setFlags(Flags flags)
	{
		mFlags = flags;
	}

	inline const WarblerFeedWindow::WarblerTweets& WarblerFeedWindow::getWarblerTweets() const
	{
		return mWarblerTweets;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
