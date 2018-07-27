// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/UnsignedFloat.h"

#include <boost/optional.hpp>
#include <boost/container/flat_map.hpp>

namespace qsf
{
	namespace ai
	{
		/**
		* An Area type defines the rules for how entities may move along a type of area used inside AI navigation maps.
		* Currently it allows fine tuning a cost factor to apply during path planning and a speed factor to apply during path execution.
		* It also tracks which other types are possible candidates to automatically connect to.
		* These are the compatible types and define the legal options when using the auto connect feature.
		* These are ignored when explicitly connecting nodes inside a street crossing because the user has full control here.
		* But when using the auto connect feature, the intersected lane is determined by proximity and we need to be able to exclude unwanted connections.
		*/
		class AreaType
		{
		public:
			// Internal helper class containing the speed effects of driving along an area as a combination of an optional absolute maximum and an optional factor to apply.
			class SpeedImpact
			{
			public:
				boost::optional<UnsignedFloat> mMaximumLimit;
				boost::optional<UnsignedFloat> mFactorToApply;
			};

			// Default c'tor is needed because it is being serialized and created inside a vector.
			// Since there is no setter for the id by design it is expected that such a default created area is assigned with a correct one later one.
			explicit AreaType(unsigned int id = getUninitialized<unsigned int>());

			// unique ID access
			unsigned int getId() const;

			// Returns whether this area type was created from real data and has valid information.
			// Currently, since these are stored in a linear array it may actually hold unused dummy entries if there are holes in the area ids used.
			bool isValid() const;

			const std::string& getName() const;
			void setName(const std::string& name);

			void addCostFactor(unsigned int moverType, bool facingEndNode, UnsignedFloat factor);
			void addSpeedFactor(unsigned int moverType, UnsignedFloat factor);
			void addSpeedLimit(unsigned int moverType, UnsignedFloat limit);

			/**
			* Returns a cost multiplier applied during path search for when an entity of type moverType is going through an area of this type in the indicated facing.
			* No return value means the entity can't use this area during planning.
			*/
			const UnsignedFloat* tryGetCostFactorFor(unsigned int moverType, bool facingEndNode) const;

			/**
			* Returns whether users of the given type may move here at all in either direction.
			*/
			bool canBeUsedAtAll(unsigned int moverType) const;

			/**
			* Returns a cost multiplier applied during path execution for when an entity of type moverType is going through an area of this type in the indicated direction.
			* No return value means no speed adaptation.
			*/
			const SpeedImpact* tryGetSpeedImpactFor(unsigned int moverType) const;

			// Determine whether an area type is compatible that means that these might typically be connected.
			bool isAreaTypeCompatible(unsigned int areaType) const;
			// Add a compatible area type
			void addCompatibleType(unsigned int areaType);
		private:
			typedef std::pair<unsigned int, bool> DataKey; // key to be used for the maps contained here using mover type as first and direction as second parameter
			typedef boost::container::flat_map<DataKey, UnsignedFloat> CostMap;
			typedef boost::container::flat_map<unsigned int, SpeedImpact> SpeedMap;
			typedef boost::container::flat_set<unsigned int> CompatibleTypeMap;

			unsigned int mId; // unique type id
			std::string mName; // plain text name for debugging and editing purposes

			CostMap mCostFactors; // The cost factors to be used during planning
			SpeedMap mSpeedFactors; // The speed factors to be applied during movement
			CompatibleTypeMap mCompatibleTypes; // The types that are considered good for directly connecting to. This is only considered when using the Auto-Connect feature.
		};
	}
}

#include "qsf_ai/worldModel/AreaType-inl.h"
