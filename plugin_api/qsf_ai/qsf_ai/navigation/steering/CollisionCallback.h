// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/NavigationComponent.h"

#include <qsf/time/Time.h>


namespace qsf
{
	class Entity;

	namespace ai
	{
		// When approaching a collision or when already in a collision, this callback is queried for a reaction
		class QSF_AI_API_EXPORT CollisionCallback
		{
		public:
			// Reactions sorted by severity of the change - from ignoring collision, letting steering system determine the best action to braking before a potential collision or stop immediately (collided and stuck)
			enum Reaction
			{
				IGNORE,
				HANDLE_BY_STEERING,
				COME_TO_HALT,
				STOP_IMMEDIATELY,
			};

			virtual ~CollisionCallback();

			/**
			*  @brief
			*    Allows a game specific reaction on the collision of BulletCollisionComponents
			*    Might trigger health drops or explosions in game, as well as return
			*    Reaction::IGNORE to ignore collision in steering up to Reaction::STOP_IMMEDIATELY
			*    to stop immediately in place.
			*    Detail: will only be called if the map features a PhysicsWorldComponent.
			*/
			virtual Reaction onEntityCollided(Entity& entity, Entity& other) = 0;

			/**
			*  @brief
			*    Allows a game specific reaction to a potential projected collision between entities.
			*    The default behavior in this case is defined by the collision avoidance measures set in the navigation component.
			*    If this allows either stopping or evading (which is the regular configuration) the collision won't happen normally.
			*    Instead the entities will do their best to avoid a collision given their respective settings.
			*    The time parameter is an absolute time stamp when the entities would collide if both wouldn't react at all and keep moving straight along their current orientation
			*    and there would not be any other collision earlier.
			*    It is therefore only an indication but it doesn't mean in general that the entities will actually collide after that time span.
			*    Returns whether the entity should potentially react to this collision or ignore it.
			*/
			virtual bool shouldPotentialCollisionBeConsidered(Entity& entity, Entity& other, Time when) = 0;

			/**
			*  @brief
			*    Allows a game specific reaction to a projected collision between entities.
			*    The main difference to shouldPotentialCollisionBeConsidered is that this is the most critical actual collision that the entity will react to.
			*    The default behavior in this case is defined by the collision avoidance measures set in the navigation component.
			*    If this allows either stopping or evading (which is the regular configuration) the collision won't happen normally.
			*    Instead the entities will do their best to avoid a collision given their respective settings.
			*    The time parameter is an absolute time stamp when the entities would collide if both wouldn't react at all and keep moving straight along their current orientation.
			*    It is therefore only an indication but it doesn't mean in general that the entities will actually collide after that time.
			*    Returns whether the entity should react to this collision or ignore it.
			*/
			virtual bool shouldReactToCollision(Entity& entity, Entity& other, Time when) = 0;
		};


		// inline implementation
		inline CollisionCallback::~CollisionCallback()
		{}
	}
}
