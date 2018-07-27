// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>
#include <qsf/time/Time.h>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
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
	*  @remarks
	*    EMERGENCY 5 spawn point component
	*
	*  @brief
	*    EMERGENCY 5 entities can spawn from this point. It has a list of valid entities which can spawn from it.
	*/
	class EM5_API_EXPORT SpawnPointComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::SpawnPointComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Spawn an random entity from a given pool
		*/
		static qsf::Entity* spawnRandomAt(const qsf::Transform& transform, qsf::Map& map, const std::string& spawnPoolName, bool checkCollisions = true);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit SpawnPointComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~SpawnPointComponent();

		inline const std::vector<std::string>& getList() const;
		inline void setList(const std::vector<std::string>& list);

		/**
		*  @brief
		*    Spawn an random entity from the list. Keeps watch of all spawn percentages and returns the new spawned entity
		*/
		qsf::Entity* spawnRandom(bool checkCollisions = true, const qsf::Time& fadeTime = qsf::Time::fromSeconds(1.0f)) const;

		/**
		*  @brief
		*    Check if an entity could spawn with a random spawn
		*
		*  @param[in] foreignMap
		*    We can choose an other map where we want to check the collision
		*
		*  @note
		*    If we have different collision geometry on different spawns we cannot assure that no collision will happen
		*/
		bool canSpawnWithoutCollision(qsf::Map& foreignMap);


	//[-------------------------------------------------------]
	//[ Public virtual em5::SpawnPointComponent methods       ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool isEntry() const   { return false; }
		inline virtual bool isExit() const    { return false; }
		inline virtual bool isTransit() const { return false; }


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::vector<std::string> mList;			///< Array of spawnable entity names; exact meaning depends on sub-class implementation (e.g. prefab local asset names)


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/SpawnPointComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::SpawnPointComponent)
