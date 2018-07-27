// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/gui/GuiDocument.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class OrderInfo;
}
namespace Rocket
{
	namespace Core
	{
		class ElementText;
	}
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
	*    EMERGENCY 5 help window HUD element
	*/
	class EM5_API_EXPORT HelpWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit HelpWindow(qsf::GuiContext& guiContext);
		virtual ~HelpWindow();
		void showHelpPages(const OrderInfo& orderInfo);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct UnitHelpPageData
		{
			uint32 pageOffset;	// Offset for the page number from which this instance counts
			std::vector<std::string> helpPages;
			std::vector<uint32> commandButtonIds;
			std::string unitIcon;
			bool isPerson;
		};

		struct UnitButtonData
		{
			Rocket::Core::Element* buttonElement;
			Rocket::Core::Element* mImageElement;
		};


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupGui();
		void onReloadGui();
		void updateCommandButtons(const UnitHelpPageData& unitHelpPageData);
		void setupUnitTypeButtons();

		/**
		*  @brief
		*    Jump to the corresponding help screen
		*/
		void jumpToCommandPage(const std::string& helpPageName);
		void jumpToUnitCommandPage(int unitId);

		void updatePageCountDisplay();
		void updateHelpScreen();

		std::vector<uint32> getCommandListForOrderInfo(const OrderInfo& orderInfo);

		void changeCurrentPage(bool advance);
		void updateCurrentUnitNumber();
		void updateCurrentUnitNumber(uint32 newUnitNumber);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<UnitHelpPageData>		mHelpPages;
		uint32								mCurrentPageNumber;
		uint32								mPageNumber;				// How many pages exists
		uint32								mCurrentUnitNumber;			// The index of the current unit for which help pages should be shown
		Rocket::Core::Element*				mCommandButtonContainer;
		std::vector<Rocket::Core::Element*>	mButtons;					///< Holds all command buttons (ordering is the same as by the ID specified in the document)
		std::vector<UnitButtonData>			mUnitButtons;				///< Holds all unit buttons (ordering is the same as by the ID specified in the document)
		bool								mCurrentInputBlockedState;

		Rocket::Core::ElementText*			mPageCountTextElement;
		Rocket::Core::Element*				mHelpImageElement;
		Rocket::Core::ElementText*			mHelpTextElement;
		Rocket::Core::ElementText*			mHelpTitleTexElement;
		Rocket::Core::Element*				mLeftArrowElement;
		Rocket::Core::Element*				mRightArrowElement;

		uint32								mCurrentOrderInfoId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
