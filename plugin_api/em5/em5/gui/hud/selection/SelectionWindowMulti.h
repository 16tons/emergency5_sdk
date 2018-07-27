// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/selection/SelectionManager.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GameComponent;
	class CommandableComponent;
}
namespace qsf
{
	class Entity;
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
	*    EMERGENCY 5 in-game selection window
	*/
	class EM5_API_EXPORT SelectionWindowMulti : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit SelectionWindowMulti(qsf::GuiContext& guiContext);


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
		/**
		*  @brief
		*    Will be call by selection change
		*/
		void onSelectionChange(const qsf::MessageParameters& parameters = qsf::MessageParameters());

		/**
		*  @brief
		*    Updates the health bars
		*/
		void onHealthUpdate();

		/**
		*  @brief
		*    Returns true if the commandableComponent from local player
		*/
		bool isUnitFromLocalPlayer(const CommandableComponent& commandableComponent);

		/**
		*  @brief
		*    Performs the left click
		*/
		void onLeftMouseButton(Rocket::Core::Element* element);

		/**
		*  @brief
		*    Performs the right click
		*/
		void onRightMouseButton(Rocket::Core::Element* element);

		/**
		*  @brief
		*    Get the ID set of a selected entity kind
		*/
		SelectionManager::IdSet getAllSameUnits(qsf::Entity& entity);

		/**
		*  @brief
		*    Show to the next 6 units
		*/
		void showPage(uint32 page);

		void updateCommandBarButtonVisibility();
		void updateCommandBarVisibility();

		/**
		*  @brief
		*    React on the camera follow changed message and update the GUI button
		*/
		void refreshCameraFollowButton(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const char HEALTHBAR_COLOR_NORMAL[];					///< Health bar background color for normal state
		static const char HEALTHBAR_COLOR_INJURED[];				///< Health bar background color for injured state


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool								mCommandToggleState;
		GameComponent*						mGameComponent;
		qsf::MessageProxy					mMessageProxy;
		qsf::MessageProxy					mFollowCameraEntityChangedProxy;
		qsf::JobProxy						mUpdateJobProxy;
		uint32								mCurrentPage;			///< Holds the current page number
		uint32								mMaxPage;				///< Holds the max page number
		Rocket::Core::Element*				mSelectionBoxes;		///< This is the rocket element which contains the unit boxes
		std::vector<Rocket::Core::Element*>	mSelectedUnits;			///< Holds the selected units
		boost::signals2::connection			mNumberOfActiveButtonsChangedSignalConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
