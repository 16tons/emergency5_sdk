// Copyright (C) 2012-2018 Promotion Software GmbH


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

#include <boost/signals2.hpp>
#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Command;
	class CommandableComponent;
	class OrderInfo;
	class SelectionManager;
}
namespace qsf
{
	class Map;
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
	*    EMERGENCY 5 in-game command window
	*/
	class EM5_API_EXPORT CommandWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void ()> NumberOfActiveButtonsChanged;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static qsf::StringHash HELP_BUTTON_ID;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit CommandWindow(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Enable a button
		*
		*  @param[in] commandId
		*    The command id of the button
		*  @param[in] state
		*    Indicates if the button should be enabled or not
		*/
		void enableButton(const qsf::StringHash commandId, bool enabled);

		/**
		*  @brief
		*    Enable all buttons
		*
		*  @param[in] state
		*    Indicates if all buttons should be enabled or not
		*/
		void enableAllButtons(bool enabled);

		/**
		*  @brief
		*    Let an button blink
		*
		*  @param[in] commandId
		*    The command id of the button set the blink state
		*  @param[in] blink
		*    Indicates if the button should blink or not
		*/
		void blinkButton(const qsf::StringHash commandId, bool blink);

		/**
		*  @brief
		*    Return the number of active buttons
		*/
		uint32 getNumberOfActiveButtons() const;

		void activateButtonByIndex(int index);
		void deactivateButtons();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onSelectionChange(const qsf::MessageParameters& parameters = qsf::MessageParameters());
		void updateCommandButtons(const std::vector<uint32>& commandVector);
		void getSharedCommandIds(boost::container::flat_set<uint32>& outCommandIds);
		bool getLocalPlayerUnits(SelectionManager& selectionManager, const qsf::Map& map, std::vector<const CommandableComponent*>& resultList);
		bool isUnitFromLocalPlayer(const CommandableComponent& commandableComponent);
		bool showCommandAsButton(const Command& command);
		void updateEquipmentInformation();
		const OrderInfo* getSingleSelectionOrderInfo() const;
		void activateCommandFromElement(Rocket::Core::Element& element);

		Rocket::Core::Element* getButtonElementForCommand(const qsf::StringHash commandId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32								mNumberOfActiveButtons;		///< Holds the number of active buttons
		qsf::MessageProxy					mMessageProxy;
		qsf::JobProxy						mUpdateJobProxyEquipmentInformation;
		std::string							mCommandName;
		Rocket::Core::Element*				mCommandButtonContainer;
		std::vector<Rocket::Core::Element*>	mButtons;					///< Holds all action buttons (Ordering is the same as by the id specified in the document)
		BlinkButton							mBlinkButton;				///< Used to let a button "blink". Currently this is only needed for the tutorial and there only one button should be blink at the same time
		bool								mIsSingleSelection;			///< Is true if only one unit selected
		boost::container::flat_map<uint32, bool> mIconFileExists;		///< Key is hash of icon name, value tells you whether the icon file exists


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
