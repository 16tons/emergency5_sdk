// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

namespace qsf
{
	class Entity;

	namespace ai
	{
		class NavigationComponent;

		class QSF_AI_API_EXPORT SpeedLimitFunctor
		{
		public:
			virtual ~SpeedLimitFunctor();

			/**
			*  @brief
			*    Speed Limit functor main method - shall return maximum speed (in world units per second) for the navigation component passed.
			*    The speed that would be applied by default is passed as second parameter.
			*    This already contains the impact of the ground type (via the edited area type).
			*    Might take map region, weather, health status, NPC type etc into account.
			*    For no further limitation, return the value passed as current default.
			*/
			virtual UnsignedFloat getSpeedFor(const NavigationComponent& navi, UnsignedFloat speedLimit) const = 0;

			/**
			*  @brief
			*    Speed Limit functor auxiliary method - shall return the distance to keep between the active navigation component and the other entity.
			*    The default distance this navi keeps ahead is given in the field defaultDistance
			*    This can usually be returned. However, if a special distance should be held - like a smaller one
			*    (between damaged vehicle and tow truck while towing, for example) or a larger one (before emergency vehicles
			*    or burning vehicles) that modified distance can be returned.
			*    BEWARE! the distance *includes* the diagonal of the navigating vehicle, i.e., indicates the distance from the midpoint to the collision point.
			*/
			virtual UnsignedFloat getHaltingDistanceBefore(const NavigationComponent& navi, Entity& other, UnsignedFloat defaultDistance) const = 0;
		};


		inline SpeedLimitFunctor::~SpeedLimitFunctor()
		{}
	}
}
