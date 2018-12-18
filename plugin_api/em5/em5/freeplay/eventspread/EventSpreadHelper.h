// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/freeplay/eventspread/EventSpreadReason.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}
namespace em5
{
	class FreeplayEvent;
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
	*    Static EMERGENCY 5 event spread helper class
	*/
	class EM5_API_EXPORT EventSpreadHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Set an event flag to the entity with an correct event reason
		*
		*  @param[in] freeplayEvent
		*    Is the freeplay event that should apply on the entity, could be a null pointer
		*  @param[in] entity
		*    The entity that should receive the event ID
		*  @parem[in] eventSpreadReason
		*    The event reason that should apply on the event ID component
		*
		*  @return
		*    "true" if the event ID is created and initialized now, "false" if the freeplay event ID was already set by another event
		*/
		static bool setEventIdComponent(FreeplayEvent* freeplayEvent, qsf::Entity& entity, eventspreadreason::Reason eventSpreadReason = eventspreadreason::NO_REASON);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
