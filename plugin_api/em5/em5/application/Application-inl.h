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
	inline const boost::property_tree::ptree& Application::getCustomizedParametersPTree() const
	{
		return mCustomizedParametersPTree;
	}

	inline Game& Application::getGame() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGame, "The EM5 game instance is invalid", QSF_REACT_NONE);
		return *mGame;
	}

	inline EmergencyGui& Application::getGui() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mGui, "The EM5 GUI instance is invalid", QSF_REACT_NONE);
		return *mGui;
	}

	inline EmergencyGui* Application::tryGetGui() const
	{
		return mGui;
	}

	inline NetworkManager& Application::getNetworkManager() const
	{
		return mNetworkManager;
	}

	inline PlayerProgress& Application::getPlayerProgress() const
	{
		return mPlayerProgress;
	}

	inline AccountManager& Application::getAccountManager() const
	{
		return mAccountManager;
	}

	inline ContentAuthentication& Application::getContentAuthentication() const
	{
		return mContentAuthentication;
	}

	inline qsf::game::StoreManager& Application::getStoreManager() const
	{
		return mStoreManager;
	}

	inline bool Application::isInStartMenu() const
	{
		return mIsInStartMenu;
	}

	inline bool Application::areCheatsEnabled() const
	{
		return mCheatsEnabled;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
