// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/serialization/SerializationFormat.h"

#include <qsf/serialization/Serializer.h>
#include <qsf/asset/AssetSystemTypes.h>

#include <memory>
#include <string>

namespace qsf
{
	class AssetProxy;

	namespace ai
	{
		class WorldModel;

		/** Simple facade to read and write different kind of world models.
		* It mainly encapsulates the identification of the type and delegating the calls to the specific serializers.
		*/
		class QSF_AI_API_EXPORT WorldModelSerializer : public Serializer
		{
		public:
			// The flag needsReserialize is set to true if a version update was detected so serializing the returned nav map directly again is advised.
			static std::auto_ptr<WorldModel> deserializeFromFile(const AssetProxy& assetProxy, bool& needsReserialize);

			// Selects the target format from the filename using the standards defined in serialization/FileHeader.h
			static void serializeToFile(const WorldModel& world, const std::string& filename);
			// Serializes to the cached asset file name again
			static void reserializeToFile(const WorldModel& world);

		private:
			static std::auto_ptr<WorldModel> deserializeFromBinaryFile(const AssetProxy& assetProxy, bool& needsReserialize);

			static void serializeToBinaryFile(const WorldModel& world, const std::string& filename);
		};
	}
}
