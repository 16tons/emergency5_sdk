// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}
namespace em5
{
	class GameComponent;
	class ProgressBar;
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
	class EM5_API_EXPORT SelectionWindowVehicle : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct Seat
		{
			uint64		entityId;
			std::string imageFilename;
			std::string tooltip;	///< Not localized text-ID
			Seat() :
				entityId(qsf::getUninitialized<uint64>()),
				imageFilename("../asset/mini_icons_avatars/empty_slot.dds")
			{ }
		};

		typedef std::vector<Seat>					Seats;
		typedef std::vector<Rocket::Core::Element*> RocketCoreElements;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void updateSeats(const qsf::Entity& entity, Seats& passengerSeats, Seats& personnelSeats);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit SelectionWindowVehicle(qsf::GuiContext& guiContext);
		virtual ~SelectionWindowVehicle();
		void disableAlarmDrive();


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
		void onSelectionChange(const qsf::MessageParameters& parameters = qsf::MessageParameters());
		void updateCommandBarVisibility(bool visible = true) const;
		void updateInformationRegular();
		void updateEnergyInformation();
		void updatePersonnelSeats();
		void updateCommandBarButtonVisibility();
		void updateCommandBarState(qsf::Entity& selectedEntity);
		void refreshCameraFollowButton(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const char HEALTHBAR_COLOR_NORMAL[];
		static const char HEALTHBAR_COLOR_BURNING[];


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				mCommandToggleState;
		ProgressBar*		mProgressBar;
		GameComponent*		mGameComponent;

		qsf::MessageProxy	mMessageProxy;
		qsf::MessageProxy	mFollowCameraEntityChangedProxy;
		qsf::JobProxy		mUpdateJobProxy;

		Rocket::Core::Element*		mUnitSeatsContainer;		///< Holds the seats container
		RocketCoreElements			mUnitSeats;					///< Holds all states (Ordering is the same as by the ID specified in the document)
		Rocket::Core::Element*		mPassengerSeatsContainer;	///< Holds the passenger seats container
		RocketCoreElements			mPassengerSeats;			///< Holds all passenger seats (Ordering is the same as by the ID specified in the document)
		boost::signals2::connection	mNumberOfActiveButtonsChangedSignalConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
