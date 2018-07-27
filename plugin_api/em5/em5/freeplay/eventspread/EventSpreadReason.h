// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{
	namespace eventspreadreason
	{


		// Event spread reason
		enum Reason
		{
			NO_REASON,			///< No spread event for no reason
			EVENT_SPECIFIC,		///< This is used to call an event specific implementation (on startup for example)
			INJURY,				///< A new person was injured
			DEAD,				///< A new person was deceased
			CONTAMINATION,		///< A new person was contaminated
			FIRE,				///< A new object caught fire
			DAMAGE_VEHICLE,		///< A vehicle was damaged (not by fire)
			GANGSTER,			///< A new person is criminal
			INJURE_PERSON_AREA,	///< A new person was injured from a injure area
			HOSTAGE,			///< A person became hostage of a gangster inside a building
			BROKEN_TREE			///< A tree fell down, may bury a vehicle or person
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // eventspreadreason
} // em5
