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
	namespace container
	{


		// Use this enum to flag the linking category for the prototypeContainer
		// Eg is the process for entering an RTW defined with EntityHelper(*mPatientEntity).enterContainer(*mTargetEntity, ContainerTypes::CONTAINERTYPE_PASSENGER);
		enum ContainerTypes
		{
			CONTAINERTYPE_PASSENGER,	// Used for normal passengers
			CONTAINERTYPE_SQUAD,		// Used for squads in vehicles
			CONTAINERTYPE_TRAPPED,		// Used for persons, trapped in buildings
			CONTAINERTYPE_PROGRESS,		// Used for persons, interacting with the container (eg. Firefighter units with buildings)
			CONTAINERTYPE_GANGSTER,		// Mark the person as gangster
			CONTAINERTYPE_HOSTAGE		// Mark the person as hostage
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	}	// container
} // em5
