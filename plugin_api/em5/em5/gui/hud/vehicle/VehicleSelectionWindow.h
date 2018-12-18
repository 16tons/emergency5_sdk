// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/helper/BlinkButton.h"
#include "em5/game/units/Department.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/message/MessageProxy.h>


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
	*    Vehicle selection window
	*
	*  @note
	*    - This is the second window in unit ordering, where you select the specific vehicle to order
	*    - Overview over unit ordering GUI:
	*        1. VehicleOrderBar
	*        2. VehicleSelectionWindow
	*        3. VehicleOrderWindow
	*/
	class EM5_API_EXPORT VehicleSelectionWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit VehicleSelectionWindow(qsf::GuiContext& guiContext);
		virtual ~VehicleSelectionWindow();

		/**
		*  @brief
		*	Let an item in the list blink
		*
		*  @param[in] itemId
		*    The ID of the item which should enabled
		*  @param[in] state
		*    Indicates if the item should enabled or not
		*/
		void enableItem(uint32 itemId, bool state);

		/**
		*  @brief
		*	Enable or disable the close button
		*/
		void enableCloseButton(bool enable);

		/**
		*  @brief
		*	Let an item in the list blink
		*
		*  @param[in] itemId
		*    The ID of the item which should blink
		*  @param[in] blink
		*    Indicates if the item should blink or not
		*/
		void blinkItem(uint32 itemId, bool blink);

		/**
		*  @brief
		*	Return the current department string
		*/
		units::Department getCurrentDepartment() const;

		/**
		*  @brief
		*	Reset the current department string
		*/
		void resetCurrentDepartment();

		/**
		*  @brief
		*	Build up all the content for the window by hand
		*/
		void setupWindow(units::Department department, float scrollTo = 0.0f);

		void selectUnitByIndex(int i);


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
		void updateOrderSelection(const qsf::MessageParameters& parameters);
		void pressedElement(Rocket::Core::Element& pressedElement);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		BlinkButton			mBlinkButton;			///< Is used to let an item blink
		units::Department	mCurrentDepartment;		///< Holds the current shown department
		size_t				mCurrentEntryCount;		///< Holds the number of elements to be shown for the department
		qsf::MessageProxy	mUpdateSelectionProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
