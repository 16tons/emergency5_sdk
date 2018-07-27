// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline const std::string& Player::getName() const
	{
		return mName;
	}

	inline void Player::setName(const std::string& name)
	{
		mName = name;
	}

	inline uint32 Player::getPlayerIndex() const
	{
		return mIndex;
	}

	inline void Player::setPlayerIndex(uint32 playerIndex)
	{
		mIndex = playerIndex;
	}

	inline uint16 Player::getTeamId() const
	{
		return mTeamId;
	}

	inline void Player::setTeamId(uint16 teamId)
	{
		mTeamId = teamId;
	}

	inline bool Player::isPlayerLocal() const
	{
		return mIsLocal;
	}

	inline VersionSwitch::Edition Player::getGameEdition() const
	{
		return mGameEdition;
	}

	inline qsf::Color4 Player::getPlayerColor() const
	{
		return mPlayerColor;
	}

	inline void Player::setPlayerColor(const qsf::Color4& color)
	{
		mPlayerColor = color;
	}

	inline uint32 Player::getUserData() const
	{
		return mUserData;
	}

	inline void Player::setUserData(const uint32 userData)
	{
		mUserData = userData;
	}

	inline qsf::CameraComponent* Player::getMainCameraComponent() const
	{
		return mMainCameraComponent.get();
	}

	inline Player::CameraType Player::getCameraType() const
	{
		return mCameraType;
	}

	inline qsf::CameraComponent* Player::getCameraComponent() const
	{
		return mCameraComponent.get();
	}

	inline const glm::vec3& Player::getViewPosition() const
	{
		return mViewPosition;
	}

	inline void Player::setViewPosition(const glm::vec3& viewPosition)
	{
		mViewPosition = viewPosition;
	}

	inline UnitPool& Player::getUnitPool() const
	{
		QSF_CHECK(nullptr != mUnitPool, "Player has no unit pool", QSF_REACT_THROW);
		return *mUnitPool;
	}

	inline uint32 Player::getScore() const
	{
		return mScore;
	}

	inline void Player::addScore(int32 score)
	{
		if (score < 0 && mScore < static_cast<uint32>(-score))
			setScore(0);
		else
			setScore(mScore + score);
	}

	inline uint32 Player::getCredits() const
	{
		return mCredits;
	}

	inline void Player::addCredits(int32 credits)
	{
		if (credits < 0 && mCredits < static_cast<uint32>(-credits))
			setCredits(0);
		else
			setCredits(mCredits + credits);
	}

	inline void Player::toggleDebugFlag(DebugFlag flag)
	{
		#ifndef ENDUSER
			mDebugFlags.invert(flag);
			refreshDebug();
		#endif
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
