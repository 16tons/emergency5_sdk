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
	namespace action
	{


		/** EM5 action priority definitions */
		enum Priority
		{
			NONE = 0,					///< No action at all
			IDLE = 10,					///< Pure idle action

			DAILY_ROUTINE = 20,			///< Daily routine action

			DISTRACTION_HARMLESS = 30,	///< Distraction action of a harmless kind, like a point of interest
			DISTRACTION_SOCIAL = 40,	///< Distraction by social interaction
			DISTRACTION_DANGER = 50,	///< Distraction by a potential dangerous threat

			AUTOMATISM_LOW = 60,		///< Self-injected automatism for units, with low priority
			AUTOMATISM_STD = 70,		///< Self-injected automatism for units, with medium priority
			AUTOMATISM_HIGH = 80,		///< Self-injected automatism for units, with high priority

			DANGER_IMMINENT = 100,		///< Reaction to a threat in sight, like retreat or taking cover
			DANGER_URGENT = 120,		///< Reaction to an immanent threat like an attack or direct damage

			COMMAND_LOW = 140,			///< User-injected action with low priority
			COMMAND_STD = 150,			///< User-injected action with medium priority
			COMMAND_HIGH = 160,			///< User-injected action with high priority

			BLOCKING = 200				///< Current action is thought to block all others and cannot be replaced
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // action
} // em5
