// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/factory/FreeplayEventFactory.h"

#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Color4;
	class Entity;
	class Map;
	class TransformComponent;
	namespace game
	{
		class EventTagComponent;
		class EventTagManagerComponent;
	}
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
	*    EMERGENCY 5 event helper
	*/
	class EM5_API_EXPORT EventHelper : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static qsf::Entity* findSpawnPoint(const qsf::StringHash& tag);
		static qsf::Entity* spawnEntity(qsf::Entity& spawnpoint, const qsf::Time& fadeTime = qsf::Time::fromSeconds(1.0f));

		static void copyPosition(const qsf::Entity& reference, qsf::Entity& target);

		static bool activateEventLayer(const std::string& layerName);
		static bool isLayerVisible(const qsf::Layer& layer);
		static bool calculateMapBoundingBox(const qsf::Map& map, glm::vec3& resultMin, glm::vec3& resultMax);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit EventHelper(FreeplayEvent& freeplayEvent);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~EventHelper();

		void makeMissingPerson(qsf::Entity& entity, const qsf::StringHash& injuryHash, bool isDead);
		void makeInjuredPerson(qsf::Entity& entity, const std::string& injury, bool addObjectives = true);
		void makeDrownedPerson(qsf::Entity& entity, bool addObjectives);
		void makeRescuablePerson(qsf::Entity& entity);
		void makeTweetPerson(qsf::Entity& entity, bool active);

		void buryPersonsUnderLightDebris(qsf::Entity& debrisEntity, bool makeMissing = false);
		void buryPersonsUnderHeavyDebris(qsf::Entity& debrisEntity, bool makeMissing = false);
		void buryPersonsUnderDebris(qsf::Entity& debrisEntity, uint32 objectiveId, bool makeMissing);

		qsf::Entity* createEventIconCircleArea(const qsf::Entity& entity, float radius, float maxOffset);
		void removeEventIcon(qsf::Entity& entity);

		qsf::Entity* getRandomEventEntity(bool showHidden = false);


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static bool checkSpawnPoint(qsf::Entity& targetEntity);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		FreeplayEvent& mFreeplayEvent;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/event/EventHelper-inl.h"
