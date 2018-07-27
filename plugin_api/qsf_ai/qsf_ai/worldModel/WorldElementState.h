// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <string>

namespace qsf
{
	namespace ai
	{
		/**
		* A state description of an atomic element of an AI navigation map.
		*
		* Many AI world descriptions will be made up of atomic areas (for example triangles, grid cells or traffic lane segments) and nodes placed among these that create a graph structure for planning.
		* This structure is necessary to be used by the default pathfinding and steering for example with the traffic lane representation.
		*
		* These areas and nodes have some common state information to guide movement along them.
		* For example they may be blocked for a short or a longer amount of time for example.
		* There is no exact definition of what this means in amount of time but in the semantics of how these flags are used.
		* Blocking long term means that entities should not consider this element or node for pathfinding and instead find another way around it.
		* They will thus not wait there and it will be a surprise if the possibility opens up again.
		* Temporarily blocked elements or nodes however will be used by entities to plan their paths and they will instead wait in front of one when they reach it and it is still blocked.
		* Consider this when deciding which value to use.
		*
		* The soon (un)blocked states are meant for the typical yellow signal situations at traffic lights while the stop once value is the state of a halting line.
		* There is an additional distinction between whether any blocking information is a mere convention or a physical restriction.
		*/
		namespace worldElement
		{

			// Attention! Order is important here as many tests inside the navigation systems rely on the rough monotonic semantic increase in urgency to stop.
			// Whenever you change something here be sure to check every manual comparison if it would be still valid.
			enum StateType
			{
				FREE,
				SOON_BLOCKED, // Free but will be blocked soon, typical yellow signal situation
				STOP_ONCE, // Move along but come to a halt at this point, typical halting line situation
				SOON_UNBLOCKED, // Blocked but will become unblocked soon, typical read and yellow light situation
				TEMPORARILY_BLOCKED,
				LONG_TERM_BLOCKED,
			};

			static const unsigned int NUM_STATES = LONG_TERM_BLOCKED + 1;

			// Static conversion to and from string for serialization purposes
			StateType parseFromString(const std::string& text);
			std::string toString(StateType state);

			// Tiny struct combining the type of state with the flag whether a contained restriction is a physical limitation or a limitation by convention.
			// Technically the flag has no effect when the type is free.
			class State
			{
			public:
				State(StateType type, bool physical);
				State(); // default c'tor is necessary for use with serialization. Creates a free world state that does not limit anything

				static State FREE_STATE;
				static State SOON_BLOCKED_STATE;
				static State SOON_UNBLOCKED_STATE;
				static State TEMPORARILY_BLOCKED_STATE;

				StateType mType;
				bool mPhysicalRestriction;

				// helper operator to compare two states for the urgency to stop for them
				// Physical restrictions are more severe than non physical (conventions), FREE is basically no restriction at all otherwise the enum types are already ordered by severity
				bool isLessUrgent(const State& other) const;
			};
		}
	}
}

#include "qsf_ai/worldModel/WorldElementState-inl.h"
