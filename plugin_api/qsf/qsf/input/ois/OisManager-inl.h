// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline OIS::Keyboard* OisManager::getOisKeyboard() const
	{
		return mOisKeyboard;
	}

	inline OIS::Mouse* OisManager::getOisMouse() const
	{
		return mOisMouse;
	}

	inline OIS::JoyStick* OisManager::getOisJoystick() const
	{
		return mOisJoystick;
	}

	inline bool OisManager::hasOisInstances() const
	{
		return (nullptr != mOisInputManager);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
