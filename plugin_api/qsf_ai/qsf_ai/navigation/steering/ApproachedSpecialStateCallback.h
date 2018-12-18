// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/worldModel/WorldElementState.h"
#include "qsf_ai/base/UnsignedFloat.h"

namespace qsf
{
	class Entity;

	namespace ai
	{
		// When approaching a node or area with the special intermediate states SOON_BLOCKED / SOON_UNBLOCKED
		// you may customize the reaction of an entity by providing a specialization of this class wrapping a
		// function pointer to the SteeringSystem.
		class QSF_AI_API_EXPORT ApproachedSpecialStateCallback
		{
		public:
			// Reactions sorted by severity of the change - from none (continue moving) to stop immediately (collided and stuck)
			enum Reaction
			{
				NONE,
				COME_TO_HALT,
				STOP_IMMEDIATELY,
			};

			virtual ~ApproachedSpecialStateCallback();

			// Return the immediate reaction and do more complex and specific reactions inside this function.
			virtual Reaction onEntityApproachesNode(Entity& entity, const worldElement::State& state, UnsignedFloat distance) = 0;
			virtual Reaction onEntityApproachesArea(Entity& entity, const worldElement::State& state, UnsignedFloat distance) = 0;
			virtual Reaction onEntityInCurrentArea(Entity& entity, const worldElement::State& state) = 0;
		};


		// inline implementation
		inline ApproachedSpecialStateCallback::~ApproachedSpecialStateCallback()
		{}
	}
}
