// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline GameTimer::GameTimer(uint32 id) :
			mId(id),
			mMessageConfiguration(getUninitialized<StringHash>()),
			mPaused(false),
			mValid(true)
		{
			// Nothing here
		}

		inline GameTimer::~GameTimer()
		{
			// Nothing here
		}

		inline uint32 GameTimer::getId() const
		{
			return mId;
		}

		inline const MessageConfiguration& GameTimer::getMessageConfiguration() const
		{
			return mMessageConfiguration;
		}

		inline const Time& GameTimer::getRemainingTime() const
		{
			return mRemainingTime;
		}

		inline void GameTimer::resetTimer(const MessageConfiguration& messageConfiguration, const Time& remainingTime)
		{
			mRemainingTime = remainingTime;
			mMessageConfiguration = messageConfiguration;
			mPaused = false;
		}

		inline void GameTimer::pause()
		{
			mPaused = true;
		}

		inline void GameTimer::resume()
		{
			mPaused = false;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf
