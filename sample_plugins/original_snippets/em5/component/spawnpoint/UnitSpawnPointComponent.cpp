// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/game/Game.h"
#include "em5/game/units/OrderInfoManager.h"
#include "em5/EM5Helper.h"

#include <qsf_game/base/DistanceHelper.h>

#include <qsf/math/EnumBitmask.h>
#include <qsf/map/query/ComponentMapQuery.h>
#include <qsf/base/StringParser.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 UnitSpawnPointComponent::COMPONENT_ID = qsf::StringHash("em5::UnitSpawnPointComponent");


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	UnitSpawnPointComponent* UnitSpawnPointComponent::findUnitSpawnPoint(const qsf::StringHash& unitTypeId, UnitSpawnPointType unitSpawnPointType, const qsf::Map& map, const glm::vec3& targetPosition)
	{
		UnitSpawnPointComponent* result = nullptr;
		MatchType bestMatchType = MATCH_NONE;
		float closestDistance = 0.0f;

		for (UnitSpawnPointComponent* unitSpawnPointComponent : qsf::ComponentMapQuery(map).getAllInstances<UnitSpawnPointComponent>())
		{
			// Is this spawn point appropriate at all?
			if (unitSpawnPointComponent->isActive() && unitSpawnPointComponent->getUnitSpawnPointType() == unitSpawnPointType)
			{
				// Match type but be
				//  - the same (in this case distance is considered as well)
				//  - or better (then the distance gets calculated, but not compared)
				const MatchType currentMatchType = unitSpawnPointComponent->matchesUnitTypeId(unitTypeId);
				if (currentMatchType != MATCH_NONE && currentMatchType >= bestMatchType)
				{
					const float currentDistance = qsf::game::DistanceHelper::get2dDistance(targetPosition, unitSpawnPointComponent->getEntity());
					if ((currentMatchType > bestMatchType) || (currentDistance < closestDistance))
					{
						// Found a better unit spawn point
						result = unitSpawnPointComponent;
						bestMatchType = currentMatchType;
						closestDistance = currentDistance;
					}
				}
			}
		}
		return result;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void UnitSpawnPointComponent::serialize(qsf::BinarySerializer& serializer)
	{
		SpawnPointComponent::serialize(serializer);

		serializer.serializeAs<uint16>(mSpawnPointType);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void UnitSpawnPointComponent::onSetSimulating(bool simulating)
	{
		mMatchingUnitIds.clear();
		if (simulating)
		{
			// Build list of valid unit IDs
			mMatchingUnitIds.reserve(mList.size());	// Just a first guess for the final size
			const auto& orderInfos = EM5_GAME.getOrderInfoManager().getElements();

			for (const std::string& unitName : mList)
			{
				// Check for wildcard
				if (boost::starts_with(unitName, "all"))
				{
					qsf::EnumBitmask<uint8, units::Department> allowDepartments;	// If no bit is set, everything is allowed, else only the set bits

					// TODO(fw): Additionally, allow filtering for vehicle types (roadvehicle, boat, helicopter)
					//  -> This requires OrderInfo to deliver the vehicle type as well

					// Resolve wildcard
					std::vector<std::string> parts;
					qsf::StringParser::splitString(unitName, parts, "-");
					for (const std::string& part : parts)
					{
						if (part == "fire" || part == "firefighter" || part == "firefighters")
							allowDepartments.set(units::DEPARTMENT_FIRE);
						else if (part == "ambulance")
							allowDepartments.set(units::DEPARTMENT_AMBULANCE);
						else if (part == "police")
							allowDepartments.set(units::DEPARTMENT_POLICE);
						else if (part == "tech")
							allowDepartments.set(units::DEPARTMENT_TECH);
					}

					// Define match type
					MatchType matchType = MATCH_WILDCARD_1;
					if (allowDepartments.anySet())
					{
						matchType = static_cast<MatchType>(matchType + 1);
					}

					// Cycle through all order infos and add those matches by the wildcard
					for (auto& pair : orderInfos)
					{
						const OrderInfo& orderInfo = *pair.second;

						// Check department
						if (!allowDepartments.anySet() || allowDepartments.isSet(orderInfo.getDepartment()))
						{
							// Order info is allowed, add it
							MatchType& unitMatchType = mMatchingUnitIds[pair.first];
							if (matchType > unitMatchType)
							{
								unitMatchType = matchType;
							}
						}
					}
				}
				else
				{
					// Directly add this unit ID
					mMatchingUnitIds.emplace(qsf::StringHash(unitName), MATCH_EXACT);
				}
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	UnitSpawnPointComponent::MatchType UnitSpawnPointComponent::matchesUnitTypeId(uint32 unitTypeId) const
	{
		if (isSimulating())
		{
			const auto iterator = mMatchingUnitIds.find(unitTypeId);
			return (iterator != mMatchingUnitIds.end()) ? iterator->second : MATCH_NONE;
		}
		else
		{
			// Fallback: We have to search the list for direct matches, no wildcard support here!
			// TODO(fw): Adding wildcard support should not be a problem here
			for (const std::string& unitName : mList)
			{
				if (qsf::StringHash(unitName) == unitTypeId)
					return MATCH_EXACT;
			}
		}

		return MATCH_NONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
