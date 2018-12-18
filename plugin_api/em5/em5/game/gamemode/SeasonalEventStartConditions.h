// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


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
	*    EMERGENCY 5 Seasonal event start conditions class
	*
	*  @note
	*    The class contains data we don't want to put in a readable format. The user is not allowed to mod this.
	*/
	class SeasonalEventStartConditions
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		struct EventDetail
		{
			std::string		eventName;
			glm::ivec2		startDate;		// "day, month" pair what indicates the begin real date for the event
			glm::ivec2		endDate;		// "day, month" pair what indicates the end real date for the event
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SeasonalEventStartConditions();

		/**
		*  @brief
		*    Destructor
		*/
		~SeasonalEventStartConditions();

		std::vector<EventDetail>& getEventDetails();

		std::vector<std::string> getAllNowPossibleSeasonEvents();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::vector<EventDetail>	mEventDetails;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
