// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <em5/freeplay/event/FreeplayEvent.h>


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
	*    Abstract objective window data source interface
	*/
	class ObjectiveWindowDataSource : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		struct ObjectiveState
		{
			uint32					 id;			///< Objective ID
			uint32					 typeId;		///< Objective type ID
			Objective::ObjectiveType objectiveType;	///< Objective state
			uint32					 currentNumber;	///< Current number
			uint32					 neededNumber;	///< Needed number
			bool					 accomplished;	///< "true" if the objective is considered to be accomplished, else "false"
		};
		struct EventState
		{
			uint32				 id;			///< Event ID
			FreeplayEvent::State state;			///< Event state
			uint32				 playerIndex;	///< Player index
		};


	//[-------------------------------------------------------]
	//[ Public virtual em5::ObjectiveWindowDataSource methods ]
	//[-------------------------------------------------------]
	public:
		// Events
		virtual uint32 getNumberOfEvents() const = 0;
		virtual const EventState& getEventState(uint32 index) const = 0;
		virtual const std::string& getEventName(uint32 index) const = 0;
		// Objectives
		virtual uint32 getNumberOfObjectives(const EventState& eventState) const = 0;
		virtual const ObjectiveState& getObjectiveState(const EventState& eventState, uint32 index) const = 0;
		virtual const std::string& getObjectiveText(const EventState& eventState, uint32 index) const = 0;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		inline ObjectiveWindowDataSource();
		inline virtual ~ObjectiveWindowDataSource();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/hud/objective/ObjectiveWindowDataSource-inl.h"
