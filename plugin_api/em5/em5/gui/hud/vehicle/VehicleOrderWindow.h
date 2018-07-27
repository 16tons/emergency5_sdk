// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/helper/BlinkButton.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/message/MessageProxy.h>

#include <Rocket/Core/String.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class OrderInfo;
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
	*    Vehicle order window
	*
	*  @note
	*    - This is the third window in unit ordering, where you choose ordering options like the number of vehicles to send
	*    - Overview over unit ordering GUI:
	*        1. VehicleOrderBar
	*        2. VehicleSelectionWindow
	*        3. VehicleOrderWindow
	*/
	class EM5_API_EXPORT VehicleOrderWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const int MAX_NUMBER_OF_PASSENGER;

		enum VehicleOrderWindowButton
		{
			VEHICLEORDERWINDOW_ORDER,
			VEHICLEORDERWINDOW_BACK,
			VEHICLEORDERWINDOW_HELP,
			VEHICLEORDERWINDOW_CLOSE
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit VehicleOrderWindow(qsf::GuiContext& guiContext);
		virtual ~VehicleOrderWindow();
		void setMaximumOrderNumber(uint32 maximumOrderNumber);

		/**
		*  @brief
		*    Rebuild the window for the given orderInfo
		*/
		void setupWindow(const OrderInfo& orderInfo);

		/**
		*  @brief
		*    Let an button blink
		*
		*  @param[in] buttonId
		*    The ID of the button which should enabled
		*  @param[in] state
		*    Indicates if the button should enabled or not
		*/
		void enableButton(VehicleOrderWindowButton buttonId, bool state);

		/**
		*  @brief
		*    Enable or disable close button
		*/
		void enableCloseButton(bool enable);

		/**
		*  @brief
		*    Let an button blink
		*
		*  @param[in] buttonId
		*    The ID of the button which should blink
		*  @param[in] blink
		*    Indicates if the button should blink or not
		*/
		void blinkButton(VehicleOrderWindowButton buttonId, bool blink);

		/**
		*  @brief
		*    Enables if the player can change the amount of personal in the ordered vehicle
		*/
		void enablePersonalModification(bool enable);

		/**
		*  @brief
		*    Start an oder unit command mode for the current order info
		*/
		void startOrderMode(bool hideWindow = false);

		void closedViaEscapeKey();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct PersonalIconDetail
		{
			std::string			   iconName;	///< Icon name without path but ending (eg. "empty_slot.dds")
			OrderInfo*			   orderInfo;	///< Order info of the unit, behind the icon
			Rocket::Core::Element* element;		///< Points to the element what contains the image "

			enum ButtonState
			{
				STATE_NORMAL,
				STATE_DESELECT,
				STATE_NOT_AVAILABLE
			};
			ButtonState buttonState;
		};


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
		void setupUI();
		void onReloadGui();
		void increment(uint32 maximum);
		void decrement(uint32 minimum);
		void updatePersonalIcon(const PersonalIconDetail& personalIconDetail);
		void updateOrderWindow(const qsf::MessageParameters& parameters);
		Rocket::Core::String getStringById(VehicleOrderWindowButton buttonId) const;


	//[-------------------------------------------------------]
	//[ Private variables                                     ]
	//[-------------------------------------------------------]
	private:
		const OrderInfo*		mOrderInfo;
		Rocket::Core::Element*	mAmountSetTextElement;	// The element what shows the amount of ordered units (eg. "1")
		BlinkButton				mBlinkButton;
		uint32					mAvailableUnits;
		uint32					mAmountInHQ;
		uint32					mMaxAmountInHQ;
		uint32					mMaximumOrderNumber;

		std::vector<PersonalIconDetail>	mPersonalDetails;	///< Saves all the display information about the ordering personal
		qsf::MessageProxy				mUpdateOrderWindowProxy;

		// TODO(sw) This is an fix for the crash when going from the game to the main menu and initiate then a gui hot reloading -> The connection to the minimap was still there but the minimap instance was destroyed
		// Proper fix see QSF Bug "H301 GUI | "qsf::GuiDocument" | We need to guarantee that for each e.g. "onPostShow()" there's a "onPostHide()""
		boost::signals2::connection	mReloadGuiConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
