// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/hud/objective/ObjectiveWindowDataSource.h"


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
	*    EMERGENCY 5 objective window data source default implementation which holds and manages the live states
	*/
	class DefaultObjectiveWindowDataSource : public ObjectiveWindowDataSource
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct LiveObjectiveState : public ObjectiveState
		{
			std::string text;	///< Objective text
		};
		typedef std::vector<LiveObjectiveState> LiveObjectiveStates;

		struct LiveEventState : public EventState
		{
			std::string			name;					///< Event name hash
			LiveObjectiveStates liveObjectiveStates;	///< Live objective states of the event
		};
		typedef std::vector<LiveEventState> LiveEventStates;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline DefaultObjectiveWindowDataSource();
		inline virtual ~DefaultObjectiveWindowDataSource();
		void captureCurrentState();


	//[-------------------------------------------------------]
	//[ Public virtual em5::ObjectiveWindowDataSource methods ]
	//[-------------------------------------------------------]
	public:
		// Events
		virtual uint32 getNumberOfEvents() const override;
		virtual const EventState& getEventState(uint32 index) const override;
		virtual const std::string& getEventName(uint32 index) const override;
		// Objectives
		virtual uint32 getNumberOfObjectives(const EventState& eventState) const override;
		virtual const ObjectiveState& getObjectiveState(const EventState& eventState, uint32 index) const override;
		virtual const std::string& getObjectiveText(const EventState& eventState, uint32 index) const override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		LiveEventStates mLiveEventStates;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/hud/objective/DefaultObjectiveWindowDataSource-inl.h"
