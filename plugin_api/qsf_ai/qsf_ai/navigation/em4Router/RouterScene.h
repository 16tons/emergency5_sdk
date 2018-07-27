// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/em4Router/wrapper/actor/EActor.h"

#include <qsf/base/idgenerator/ConsecutiveIdGenerator.h>

#include <unordered_set>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class CollisionComponent;
	class Clock;

	namespace ai
	{
		class RouterComponent;
		class EActor;
		class EGenericObject;
		class EPerson;
		class EVehicle;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class QSF_AI_API_EXPORT RouterScene
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			RouterScene();
			~RouterScene();

			void startup();
			void shutdown();
			void update(const Clock& clock);

			void addActor(EActor& actor);
			void removeActor(EActor& actor);
			void showActor(EActor& actor, bool show);

			RouterComponent* getOrCreateRouterComponent(Entity& entity);

			// EScene wrapper methods
			EGenericObject* GetGenericObject(int id) const;
			EPerson* GetPerson(int id) const;
			EVehicle* GetVehicle(int id) const;

			long getGameTimerTicks();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			EActor* getActorOfTypeById(EActorType actorTypeBitmask, int actorId) const;

			void onCollisionComponentAdded(CollisionComponent& collisionComponent);
			void onCollisionComponentRemoved(CollisionComponent& collisionComponent);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			typedef std::map<int, EActor*> ActorMap;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			ActorMap mActors;								///< Actors mapped by their actor IDs (not to mix up with entity IDs)
			ConsecutiveIdGenerator<int> mActorIdGenerator;

			std::unordered_set<uint64> mCollisionEntitiesAdded;	///< IDs of entities with newly registered collision components
			std::unordered_set<uint64> mCollisionEntitiesRemoved;	///< IDs of entities with newly deregistered collision components

			long mGameTimerTicks;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
