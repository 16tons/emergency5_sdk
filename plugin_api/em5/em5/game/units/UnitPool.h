// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/units/Department.h"
#include "em5/application/VersionSwitch.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/base/NamedIdentifier.h>

#include <vector>
#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	class DebugDrawProxy;
}
namespace em5
{
	class UnitPoolDefinition;
	class OrderInfo;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Keeps track of ordered units from the player and manages the ordering process for new units
	*    Spawning units has to register as spawning. Then they are blocked for other processes.
	*    An vehicle can only spawn personnel if the personnel is available.
	*    In case the unit died, we block the person. The resetting process takes place at every ingame day
	*/
	class EM5_API_EXPORT UnitPool : public qsf::Object
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		UnitPool();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UnitPool();

		void setGameEdition(VersionSwitch::Edition gameEdition);

		void loadStartConfiguration();
		void eraseAllRegisteredUnitsFromPool();

		bool hasUnit(const qsf::StringHash& identifier) const;

		void registerUnitInSpawn(const OrderInfo& orderInfo, int amount = 1);
		void unregisterUnitInSpawn(const OrderInfo& orderInfo, int amount = 1);

		void registerUnitInMap(const OrderInfo& orderInfo, uint64 unitEntityId);
		void unregisterUnitInMap(const OrderInfo& orderInfo, uint64 unitEntityId);

		void registerUnitBlocking(const OrderInfo& orderInfo, int amount = 1);
		void unregisterAllUnitBlocking();

		// All together in every state
		uint32 getNumberMaxAvailable(const OrderInfo& orderInfo) const;
		uint32 getNumberMaxAvailableByName(const qsf::StringHash& identifier) const;

		// Between HQ and map (trying to find a place on the spawn)
		bool isUnitAvailableInSpawn(const OrderInfo& orderInfo) const;
		uint32 getNumberInSpawning(const OrderInfo& orderInfo) const;

		// Dead or otherwise blocked. Restored at the end of the day
		uint32 getNumberBlocked(const OrderInfo& orderInfo) const;
		uint32 getNumberBlockedByName(const qsf::StringHash& identifier) const;

		// Those can be ordered by the player
		bool isUnitAvailableInHQ(const OrderInfo& orderInfo) const;
		int getNumberInHQ(const OrderInfo& orderInfo) const;

		// InUse = on map
		uint32 getNumberUnitsInUse(const OrderInfo& orderInfo) const;
		uint32 getNumberUnitsInUseByName(const qsf::StringHash& identifier) const;
		void getAllUnitsInUse(std::vector<uint64>& allUnitsInUse) const;
		const std::vector<uint64>& getUnitsInUse(const OrderInfo& orderinfo) const;
		const std::vector<uint64>& getUnitsInUseByName(const qsf::StringHash& identifier) const;

		void increaseNumberAvailable(const std::string& unitName, int amount);
		void increaseLimitForBuying(const std::string& unitName, int amount);

		bool isBuyLimitReached(const OrderInfo& orderInfo) const;
		bool isBuyLimitReached(const std::string& unitName) const;

		void getAllUnitOrderInfos(std::vector<const OrderInfo*>& outOrderInfos) const;
		void getUnitOrderInfosByDepartment(units::Department, std::vector<const OrderInfo*>& outOrderInfos) const;
		int getOriginalOrderIndexOfOrderInfoId(uint32 orderInfoId) const;	// Negative if not found

		/**
		*  @brief
		*    Extra function for handling paramedic team. Replaced the stored paramedic ID with the new one
		*
		*  @note
		*    It's important to call this function before the event spread helper acts with the old data (the playerHelper will not find the correct number of injured paramedics)
		*/
		void replaceParamedicInUse(uint64 newSingleParamedicId, uint64 oldParamedicTeamId);

		/**
		*  @brief
		*    Function for adding units to the unit pool. Is used internally.
		*    Prefer using the unitpool asset instead of adding units manually
		*/
		void addAvailableUnitByName(const qsf::NamedIdentifier& unitName, uint32 unitCount, bool allowDuplicates);

		//[-------------------------------------------------------]
		//[ Serialization                                         ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Load a unit pool from a definition structure -- only numbers of available units
		*/
		void loadFromDefinition(const UnitPoolDefinition& unitPoolDefinition);

		/**
		*  @brief
		*    Binary serialization of a unit pool -- complete serialization
		*/
		void serialize(qsf::BinarySerializer& serializer);

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Create debug output
		*/
		void addDebugOutput(qsf::DebugDrawProxy& debugDrawProxy);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct OrderInfoDetail
		{
			int mUnitsAvailable;				///< Available number of units in pool, including the ones currently in use
			int mUnitsLimitForBuying;			///< Maximum number of units in pool by buying new ones
			int mUnitsInSpawning;				///< Units currently waiting for spawning (a very temporary state)
			int mUnitsBlocked;					///< Number of "dead" units, reset at the next day
			std::vector<uint64>	mUnitsInUse;	///< Entity IDs of units currently in the map

			OrderInfoDetail();
		};
		typedef std::unordered_map<uint32, OrderInfoDetail> OrderInfoDetailMap;
		typedef std::vector<uint32> OrderInfoIdArray;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		OrderInfoDetail* getOrderInfoDetail(const OrderInfo& orderInfo);
		const OrderInfoDetail* getOrderInfoDetail(const OrderInfo& orderInfo) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		OrderInfoDetailMap	mOrderInfoDetails;
		OrderInfoIdArray	mOrderInfoIdOriginalOrder;
		VersionSwitch::Edition mGameEdition;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::UnitPool)
