// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class StartMenuBaseLayer;
	class ModificationWindow;
	class StartMenuInstalled;
	class ForcePatchDialog;
	class MaterialCacheHeatingDialog;
	class LicenseTextDialog;
	class ModificationWindowDataSource;
	class AccountInfo;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 start menu manager
	*/
	class StartMenuManager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		StartMenuManager();

		/**
		*  @brief
		*    Destructor
		*
		*  @note
		*    - Not virtual by intent
		*/
		~StartMenuManager();

		/**
		*  @brief
		*    Load the default EMERGENCY 5 start menu
		*/
		void loadGui();

		/**
		*  @brief
		*    Hide the default EMERGENCY 5 start menu
		*/
		void hideGui();

		/**
		*  @brief
		*    Destroy the EMERGENCY 5 start menu
		*/
		void destroyGui();

		void showAccountInfoWindow(bool show);

		/**
		*  @brief
		*    Return the start menu base layer window instance
		*
		*  @return
		*    Reference to the start menu base layer window instance, do not destroy the instance
		*/
		inline StartMenuBaseLayer& getStartMenuBaseLayerWindow() const;

		/**
		*  @brief
		*    Return the start menu modification window instance
		*
		*  @return
		*    Reference to the start menu modification window instance, do not destroy the instance
		*/
		inline ModificationWindow& getModificationWindow() const;

		/**
		*  @brief
		*    Return the installed start menu window instance
		*
		*  @return
		*    Reference to the installed menu window instance, do not destroy the instance
		*/
		inline StartMenuInstalled& getStartMenuInstalledWindow() const;

		/**
		*  @brief
		*    Return the modification data source instance
		*
		*  @return
		*    Reference to the modification data source instance, do not destroy the instance
		*/
		inline ModificationWindowDataSource& getModificationWindowDataSource() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    This method will be call by UpdateCheck if the patch available check done
		*/
		void onUpdateCheckFinished();

		/**
		*  @brief
		*    This method will be call by UpdateCheck if the patch available check gone wrong
		*/
		void onUpdateCheckFailed();

		/**
		*  @brief
		*    Shows the launcher or the patch window
		*
		*  @param[in] isPatchAvailable
		*    If true the start manager shows the force patch dialog
		*/
		void showLauncherOrForcePatchWindow(bool isPatchAvailable);

		void updateAccountStatus();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		StartMenuBaseLayer*				mStartMenuBaseLayerWindow;		///< Start Menu base layer Window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		StartMenuInstalled*				mStartMenuInstalledWindow;		///< Start menu installed Window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		ModificationWindow*				mModificationWindow;			///< Modification Window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		ForcePatchDialog*				mForcePatchDialog;				///< Start menu force patch dialog - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		MaterialCacheHeatingDialog*		mMaterialCacheHeatingDialog;	///< Start menu material cache heating dialog - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		LicenseTextDialog*				mLicenseTextDialog;				///< Start menu license dialog - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		ModificationWindowDataSource*	mModificationWindowDataSource;	///< Modification window datasource - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it
		AccountInfo*					mAccountInfoWindow;				///< Account info window - GUI document instance; may be a null pointer, we're responsible for destroying the instance in case we no longer need it


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/startmenu/StartMenuManager-inl.h"
