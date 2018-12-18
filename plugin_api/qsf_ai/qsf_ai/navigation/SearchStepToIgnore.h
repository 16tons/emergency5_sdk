// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BinarySerializer.h>


namespace qsf
{
	namespace ai
	{
		/**
		* Minor helper class that stores information about a move that should be ignored for the next search.
		* Simple class that is used like a struct.
		*/
		class SearchStepToIgnore
		{
		public:
			SearchStepToIgnore();
			SearchStepToIgnore(unsigned int map, unsigned int worldElement, unsigned int node);

			unsigned int mMapId;
			unsigned int mWorldElementId;
			unsigned int mNextNodeId; // may be uninitialized, which means disallowing a direct move or transition to use
		};


		// inline implementation
		inline SearchStepToIgnore::SearchStepToIgnore() :
			mMapId(getUninitialized(mMapId)),
			mWorldElementId(getUninitialized(mWorldElementId)),
			mNextNodeId(getUninitialized(mNextNodeId))
		{}

		inline SearchStepToIgnore::SearchStepToIgnore(unsigned int map, unsigned int worldElement, unsigned int node) :
			mMapId(map),
			mWorldElementId(worldElement),
			mNextNodeId(node)
		{}
	}

	namespace serialization
	{
		template <>
		struct serializer<ai::SearchStepToIgnore>
		{
			inline static void serialize(BinarySerializer& serializer, ai::SearchStepToIgnore& searchStep)
			{
				serializer & searchStep.mMapId;
				serializer & searchStep.mWorldElementId;
				serializer & searchStep.mNextNodeId;
			}
		};
	}
}
