// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		namespace worldElement
		{
			inline State::State() :
			mType(FREE),
			mPhysicalRestriction(false)
			{}

			inline State::State(StateType type, bool physical) :
			mType(type),
			mPhysicalRestriction(physical)
			{}

			inline bool State::isLessUrgent(const State& other) const
			{
				// optimized through a binary decision diagram
				if (mType < other.mType)
					return !(mPhysicalRestriction && !other.mPhysicalRestriction && mType > FREE);
					// typical condition for being less urgent, therefore almost a sure yes, just make sure that this doesn't win by 'physicality'

				// otherwise we are only clearly less urgent if the other wins by 'physicality'
				return !mPhysicalRestriction && other.mPhysicalRestriction && other.mType > FREE;
			}
		}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::worldElement::State>
		{
			inline static void serialize(BinarySerializer& serializer, ai::worldElement::State& state)
			{
				serializer.serializeAs<unsigned int>(state.mType);
				serializer & state.mPhysicalRestriction;
			}
		};
	}
}
