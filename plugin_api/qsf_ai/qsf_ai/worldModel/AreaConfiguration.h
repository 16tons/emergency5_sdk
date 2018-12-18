// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>

namespace qsf
{
	namespace ai
	{
		/**
		* Tiny helper struct, contains a map and an area ID.
		* These two are neede together to uniquely identify an area.
		*/
		class AreaConfiguration
		{
		public:
			// The default constructor is only needed for binary serialization
			explicit AreaConfiguration(uint32 mapId = getUninitialized<uint32>(), uint32 areaId = getUninitialized<uint32>());

			bool isInitialized() const; // returns whether mapId and areaId are both initialized

			union
			{
				struct
				{
#ifdef __APPLE__
	#include <TargetConditionals.h> // Defines TARGET_RT_LITTLE_ENDIAN
#endif
#if defined(WIN32) || (defined(__APPLE__) && TARGET_RT_LITTLE_ENDIAN)
					// TODO(fw): This order is okay for little endian architectures only
					uint32 mAreaId;
					uint32 mMapId;
#else
	#error Unsupported platform!
#endif
				};
				uint64 mData;
			};
		};

		// sorting and comparison mainly for inserting into STL containers
		bool operator< (const AreaConfiguration& lhs, const AreaConfiguration& rhs);
		bool operator== (const AreaConfiguration& lhs, const AreaConfiguration& rhs);
		bool operator!= (const AreaConfiguration& lhs, const AreaConfiguration& rhs);
	}
}

// "qsf::ai::AreaConfiguration" hash function for "std::unordered_map"
namespace std
{
	template <>
	struct hash<qsf::ai::AreaConfiguration>
	{
		typedef qsf::ai::AreaConfiguration argument_type;
		typedef std::size_t result_type;

		result_type operator()(const qsf::ai::AreaConfiguration& key) const
		{
			std::size_t val = 0;
			boost::hash_combine(val, key.mMapId);
			boost::hash_combine(val, key.mAreaId);
			return val;
		}
	};
}

#include "qsf_ai/worldModel/AreaConfiguration-inl.h"
