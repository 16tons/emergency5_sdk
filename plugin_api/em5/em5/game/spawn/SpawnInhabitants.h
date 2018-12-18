// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/spawn/SpawnEntities.h"


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
	*     Spawn inhabitants into the world.
	*
	*  @note
	*     - Spawning can be enabled/disabled with the GameSettingsGroup's "SpawnCivilActive" property
	*
	*  @todo
	*     - TODO(fw): Most probably this class should be replaced wth entries in the "em5::SpawnGroupManager"
	*/
	class SpawnInhabitants : public SpawnEntities
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SpawnInhabitants();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SpawnInhabitants();


	//[-------------------------------------------------------]
	//[ Protected virtual em5::SpawnEntities methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateSpawning() override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
