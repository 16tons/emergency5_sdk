// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{



		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline SteeringSystem::SteeringSystem(Prototype* prototype) :
			StandardSystem<NavigationComponent, SteeringSystem>(prototype, NAME),
			mDebugSettings(nullptr),
			mFastDebugDraw(nullptr),
			mApproachedSpecialStateHandler(nullptr),
			mCollisionCallback(nullptr),
			mSpeedLimits(nullptr)
		{
			// Nothing here
		}

		inline void SteeringSystem::createDebugOutput(const NavigationComponent&) const
		{
			// Is all done inside the main code as we have much additional data that is needed to be visualized and that is only available temporarily during the update
		}

		inline void SteeringSystem::setApproachedSpecialStateHandler(ApproachedSpecialStateCallback* handler)
		{
			mApproachedSpecialStateHandler = handler;
		}

		inline void SteeringSystem::setCollisionHandler(CollisionCallback* handler)
		{
			mCollisionCallback = handler;
		}

		inline void SteeringSystem::setSpeedLimitFunctor(const SpeedLimitFunctor* speedLimit)
		{
			mSpeedLimits = speedLimit;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
