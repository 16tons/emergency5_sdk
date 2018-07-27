// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>


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
	*    EMERGENCY 5 Mac App Store window
	*/
	class AppStoreWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit AppStoreWindow(qsf::GuiContext& guiContext);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;
		virtual bool onPreShow() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void askForPurchase(const qsf::NamedIdentifier& productId, const std::string& title, const std::string& text);

		void switchToWindowMode();
		void showConnectionMessage();

		void setupStoreManagerListeners();
		void removeStoreManagerListeners();

		void onPurchaseSuccess(uint32 productId);
		void onPurchaseRestore(uint32 productId);
		void onPurchaseCancel(uint32 productId);
		void onPurchaseError(uint32 productId);

		void onRestoreSuccess();
		void onRestoreFailed();

		void onOkayForPurchase(const qsf::NamedIdentifier& productId);
		void onOkayForHide();
		void onOkayForRestart();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mRestartNeeded;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
