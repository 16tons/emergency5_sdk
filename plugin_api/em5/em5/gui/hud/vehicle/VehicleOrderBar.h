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

#include <Rocket/Core/String.h>


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
	*    Vehicle order bar window
	*
	*  @note
	*    - This is the first window in unit ordering, where you select the unit category (fire dept, police, ambulance or tech)
	*    - Overview over unit ordering GUI:
	*        1. VehicleOrderBar
	*        2. VehicleSelectionWindow
	*        3. VehicleOrderWindow
	*/
	class EM5_API_EXPORT VehicleOrderBar : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum VehicleOrderBarButton
		{
			VEHICLEORDERBARBUTTON_FIREDEPARTMENT,
			VEHICLEORDERBARBUTTON_AMBULANCE,
			VEHICLEORDERBARBUTTON_POLICEDEPARTMENT,
			VEHICLEORDERBARBUTTON_TECH
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit VehicleOrderBar(qsf::GuiContext& guiContext);

		void setEnabled(bool enabled);

		void enableButton(bool state, VehicleOrderBarButton buttonID);
		void blinkButton(bool state, VehicleOrderBarButton buttonID);
		void setButtonVisible(bool state, VehicleOrderBarButton buttonID);

		void deselectAll();

		void clickFireDepartment();
		void clickAmbulanceDepartment();
		void clickPoliceDepartment();
		void clickTechDepartment();


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
		void setupUI();
		void onReloadGui();

		Rocket::Core::String getStringByID(VehicleOrderBarButton buttonID);
		void showVehicleSelection(const std::string& elementName, units::Department selectionDepartment);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool					mEnabled;
		BlinkButton				mBlinkButton;			///< Needed for tutorial
		Rocket::Core::Element*	mOrderBarContainer;		///< Holds the rocket element which with all buttons

		// TODO(sw) This is an fix for the crash when going from the game to the main menu and initiate then a gui hot reloading -> The connection to the minimap was still there but the minimap instance was destroyed
		// Proper fix see QSF Bug "H301 GUI | "qsf::GuiDocument" | We need to guarantee that for each e.g. "onPostShow()" there's a "onPostHide()""
		boost::signals2::connection	mReloadGuiConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
