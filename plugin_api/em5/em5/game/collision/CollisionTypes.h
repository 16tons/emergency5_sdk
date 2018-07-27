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
	namespace collision
	{


		// TODO(co) Semantics is wrong, "filter group" = "that's me, my category"
		/**
		* @brief Specification of the collision flags used inside EMERGENCY 5.
		* These flags are used as single bit values that can be combined into a bitmask to signal of which type a collision object used inside EM5 is.
		* A combination of these flags is used as bitmask as well as collision filter for QSF collision components as well as AI collision avoidance settings.
		* @note These values are combined into a short by convention defined by bullet. Respect the limit when adding new values.
		*
		* These values should always stay compatible with the defaults used by bullet defined in BulletCollision/BroadphaseCollision/btBroadphaseProxy.h as enum CollisionFilterGroups.
		* This is because bullet might assume some of these internally in some operations
		*/
		enum Types
		{
			// Values defined by bullet and how we use them in EM5
			DEFAULTFILTER		= 1 << 0,	// Default is used as real physical collision inside EM5. Besides this typical (=> default) type of collision objects we also use collisions for testing perceptions for the AI for example
			STATICFILTER		= 1 << 1,	// everything that is static
			KINEMATICFILTER		= 1 << 2,	// everything that may move
			DEBRISFILTER		= 1 << 3,	// unclear
			SENSORTRIGGER		= 1 << 4,	// everything that is no physical collision but a stimulus for the perception system
			CHARACTERFILTER		= 1 << 5,	// player controlled entities

			// Custom EM5 values start

			// depth boundaries for the water. The water depth are already mixed, inclusive types. every medium water entity is also a shallow water entity for example
			SHALLOWWATERFILTER	= 1 << 6,	// only the diver may move here, but he doesn't swim. Persons don't drown but also don't move here
			MEDIUMWATERFILTER	= 1 << 7 | SHALLOWWATERFILTER, // player motorboat and diver, but not the big ships like the player cruiser, persons drown here
			DEEPWATERFILTER		= 1 << 8 | MEDIUMWATERFILTER, //all boats including the player cruiser, persons drown here

			// danger objects
			BURNINGFIREFILTER	= 1 << 9,
			TOXICCLOUDFILTER	= 1 << 10,

			VORONOIFILTER		= 1 << 11,	// voronoi tester are the objects that create the AI maps
			PROJECTILEFILTER	= 1 << 12,	// bullets, thrown grenades or rocks or molotov cocktails
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // collision
} // em5
