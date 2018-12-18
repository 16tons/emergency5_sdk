// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/worldModel/trafficLanes/TrafficLaneWorldSerializer.h"

#include <qsf/serialization/binary/BinarySerializer.h>
#include <qsf/file/FileStream.h>

#include <memory>

namespace qsf
{
	namespace ai
	{
		class TrafficLaneWorld;

		// Interface for converting traffic lane worlds to and from binary format
		class TrafficLaneWorldBinarySerializer : public TrafficLaneWorldSerializer
		{
		public:
			// This is a global serializer version for binary archives of type traffic lane world.
			// I need to decide on the top level whether any class I just read is actually deprecated to always immediately update the files read in these cases.
			// Increase this every time you actually change anything inside the binary serialization code for traffic lane worlds.
			static const unsigned int CURRENT_VERSION;

			// Attention. The two functions are not completely symmetric since the header is already read to know which type of content is contained.
			static std::auto_ptr<TrafficLaneWorld> deserialize(BinarySerializer& serializer);
			static void serialize(const TrafficLaneWorld& world, FileStream& outputStream);
		};
	}
}
