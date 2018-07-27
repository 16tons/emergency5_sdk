// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline float FreeMoveInputHandler::getMoveSpeed() const
	{
		return mMoveSpeed;
	}

	inline float FreeMoveInputHandler::getWheelMoveSpeed() const
	{
		return mWheelMoveSpeed;
	}

	inline float FreeMoveInputHandler::getRotateSpeed() const
	{
		return mRotateSpeed;
	}

	inline const FreeMoveInputDevice& FreeMoveInputHandler::getFreeMoveInputDevice()
	{
		return *mInputDevice;
	}

	inline float FreeMoveInputHandler::getDefaultMoveSpeed() const
	{
		return 20.0f;
	}

	inline void FreeMoveInputHandler::setNullSpeedWithAltKeyEnabled(bool enable)
	{
		mNullSpeedWithAltKeyEnabled = enable;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
