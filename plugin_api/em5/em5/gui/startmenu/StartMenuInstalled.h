// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>

#include <boost/signals2.hpp>


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
	*    EMERGENCY 5 start menu installed GUI
	*/
	class StartMenuInstalled : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void ()> Login;	///< This Boost signal is emitted after the login was done


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit StartMenuInstalled(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~StartMenuInstalled();

		void onUpdateMessageBoxAnswer(bool isInstallUpdate);
		inline uint32 getNewsId() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGui();
		void onReloadGui();
		void hideAccountField();
		void showAccountField();
		void fillNewsField();
		void switchLoginFieldToOnlineMessage();
		void updateLoginButtonState();
		void startUpdateCheck();
		void onUpdateCheckFinished();
		void onUpdateCheckFailed();
		void onUpdateProgress(uint32 currentFile, uint32 totalFiles, uint64 currentBytes, uint64 totalBytes);
		void onUpdateFailed();
		void onShowPatchScreen(bool yes);
		void doLogin();
		void updateRatingReminder();
		void ratingReminderResponse(bool state);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool   mIsUpdateCheckPending;		///< "true" if an update check is currently running and we're still waiting for the result, "false" otherwise
		bool   mAutoLogin;
		bool   mUpdateCheckAlreadyDone;
		bool   mTryToLogin;
		uint32 mNewsId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/startmenu/StartMenuInstalled-inl.h"
