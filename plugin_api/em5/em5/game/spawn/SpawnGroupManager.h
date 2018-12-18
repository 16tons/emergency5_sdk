// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/spawn/SpawnGroup.h"

#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/base/manager/ElementManager.h>

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class CivilistSpawnPointComponent;
}
namespace qsf
{
	class Entity;
	class Transform;
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
	*    Spawn group manager
	*/
	class EM5_API_EXPORT SpawnGroupManager : public qsf::ElementManager<uint32, SpawnGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier CIVIL_PERSONS_DEFAULT_GROUP;		///< "CivilPersonsDefaultGroup" spawn group name
		static const qsf::NamedIdentifier CIVIL_VEHICLES_DEFAULT_GROUP;		///< "CivilVehiclesDefaultGroup" spawn group name


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SpawnGroupManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SpawnGroupManager();

		void createStartupPopulations() const;

		bool isDebugShown() const;
		void setDebug(bool debug);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		SpawnGroup* createSpawnGroup(const qsf::NamedIdentifier& id);
		void loadSpawnGroupDefinitions();

		void updateJob(const qsf::JobArguments& jobArgs);

		void updateCountersForAllGroups() const;

		void getStartupPopulationPoints(uint32 count, uint32 worldIndex, uint32 laneTypeId, std::vector<qsf::Transform>& outPoints) const;
		void createStartupPopulation(uint32 spawnGroupId, uint32 laneTypeId, const std::string& spawnPoolName) const;

		qsf::Entity* trySpawnEntityFrom(CivilistSpawnPointComponent& spawnPointComponent, uint32 spawnGroupId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy mJobProxy;

		qsf::DebugDrawProxy mDebugDrawProxy;
		bool mShowDebug;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
