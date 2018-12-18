// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline BaseHudLayer& IngameHud::getBaseHudLayer() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mBaseHudLayer, "The base HUD layer instance is invalid", QSF_REACT_NONE);
		return *mBaseHudLayer;
	}

	inline ChatWindow& IngameHud::getChatWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mChatWindow, "The chat window instance is invalid", QSF_REACT_NONE);
		return *mChatWindow;
	}

	inline MessageWindow* IngameHud::getMessageWindow() const
	{
		return mMessageWindow;
	}

	inline WarblerFeedWindow* IngameHud::getWarblerFeedWindow() const
	{
		return mWarblerFeedWindow;
	}

	inline SupervisorWindow& IngameHud::getSupervisorWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mSupervisorWindow, "The supervisor window instance is invalid", QSF_REACT_NONE);
		return *mSupervisorWindow;
	}

	inline FullscreenVideoWindow& IngameHud::getFullscreenVideoWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mFullscreenVideoWindow, "The video window instance is invalid", QSF_REACT_NONE);
		return *mFullscreenVideoWindow;
	}

	inline MiniMapWindow* IngameHud::getMiniMapWindow() const
	{
		return mMiniMapWindow;
	}

	inline UnitOverviewWindow& IngameHud::getUnitOverviewWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mUnitOverviewWindow, "The unit overview window instance is invalid", QSF_REACT_NONE);
		return *mUnitOverviewWindow;
	}

	inline SEKDroneScanWindow& IngameHud::getSEKDroneScanWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mSEKDroneScanWindow, "The SEK scan window instance is invalid", QSF_REACT_NONE);
		return *mSEKDroneScanWindow;
	}

	inline VehicleOrderBar* IngameHud::getVehicleOrderBar() const
	{
		return mVehicleOrderBar;
	}

	inline VehicleSelectionWindow& IngameHud::getVehicleSelectionWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mVehicleSelectionWindow, "The VehicleSelectionWindow instance is invalid", QSF_REACT_NONE);
		return *mVehicleSelectionWindow;
	}

	inline VehicleOrderWindow& IngameHud::getVehicleOrderWindow() const
	{
		// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
		QSF_ASSERT(nullptr != mVehicleOrderWindow, "The VehicleOrderWindow instance is invalid", QSF_REACT_NONE);
		return *mVehicleOrderWindow;
	}

	inline MenuBar* IngameHud::getMenuBar() const
	{
		return mMenuBar;
	}

	inline BuyUpgradeDataSource& IngameHud::getBuyUpgradeDataSouce() const
	{
		// It's never safe to blindly dereference a pointer - otherwise we wouldn't need a pointer at all
		QSF_ASSERT(nullptr != mBuyUpgradeDataSource, "The Buy-/Upgrade datasouce instance is invalid", QSF_REACT_THROW);
		return *mBuyUpgradeDataSource;
	}

	inline IngameHighscoreDatasource& IngameHud::getIngameHighscoreDatasource() const
	{
		// It's never safe to blindly dereference a pointer - otherwise we wouldn't need a pointer at all
		QSF_ASSERT(nullptr != mIngameHighscoreDatasource, "The online highscore datasouce instance is invalid", QSF_REACT_THROW);
		return *mIngameHighscoreDatasource;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
