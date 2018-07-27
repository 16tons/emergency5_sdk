// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/units/OrderInfo.h"

#include <qsf/logic/action/Action.h>

#include <glm/gtc/quaternion.hpp>

#include <boost/optional.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Player;
}
namespace qsf
{
	class Transform;
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
	*    EMERGENCY 5 spawn unit action
	*
	*  @note
	*    - This action is pushed to entities with spawn point components, it creates an entity of the given type and handles animation
	*/
	class EM5_API_EXPORT SpawnUnitAction : public qsf::Action
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const qsf::NamedIdentifier ACTION_ID;	///< "em5::SpawnUnitAction" unique action identifier


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Setups the newly created entity as unit vehicle and sets the initial position
		*
		*  @param[in] newOrderedUnit
		*    The newly created entity, which should be setup as a unit
		*  @param[in] spawnPointEntity
		*    The spawn point entity from which the unit should "enter" the map
		*  @param[in] orderInfo
		*    The information about the unit which was ordered
		*  @param[in] player
		*    The player, which initiated the unit ordering -> The unit will belong to this player
		*/
		static void setupCreatedEntityAsUnitVehicle(qsf::Entity& newOrderedUnit, qsf::Entity& spawnPointEntity, const OrderInfo& orderInfo, const Player& player);
		static void setupCreatedEntityAsUnitVehicle(qsf::Entity& newOrderedUnit, const qsf::Transform& transform, const OrderInfo& orderInfo, const Player& player);
		static void setupCreatedEntityAsUnitVehicle(qsf::Entity& newOrderedUnit, const OrderInfo& orderInfo, const Player& player);

		static void registerPersonnelInSpawn(const OrderInfo& orderInfo, const OrderInfo::OrderPair& orderPair, const Player& player);
		static void fillVehicleWithStartPersonnel(qsf::Entity& vehicleEntity, const OrderInfo& orderInfo, const Player& player, OrderInfo::OrderPairList* orderPairList = nullptr, uint64 spawnpointEntityId = qsf::getUninitialized<uint64>(), uint64 userData = qsf::getUninitialized<uint64>());

		/**
		*  @brief
		*    Setups the newly created entity as unit person
		*
		*  @param[in] newOrderedUnit
		*    The newly created entity, which should be setup as a unit
		*  @param[in] vehicleEntity
		*    The vehicle entity to which the unit person belongs (optional, may be a null pointer)
		*  @param[in] orderInfo
		*    The information about the unit which was ordered
		*  @param[in] player
		*    The player, which initiated the unit ordering -> The unit will belong to this player
		*  @param[in] hidden
		*    Hide the entity?
		*/
		static void setupCreatedEntityAsUnitPerson(qsf::Entity& newOrderedUnit, qsf::Entity* vehicleEntity, const OrderInfo& orderInfo, const Player& player, bool hidden = true);

		/**
		*  @brief
		*    Setups the newly created entity as unit
		*
		*  @param[in] newOrderedUnit
		*    The newly created entity, which should be setup as a unit
		*  @param[in] orderInfo
		*    The information about the unit which was ordered
		*  @param[in] player
		*    The player, which initiated the unit ordering -> The unit will belong to this player
		*/
		static void setupCreatedEntityAsUnit(qsf::Entity& newOrderedUnit, const OrderInfo& orderInfo, const Player& player);

		static void sendUnitSpawnedMessage(const qsf::Entity& newOrderedUnit, uint64 spawnpointEntityId, uint64 userData);

		static void sendUnitSpawnedPlayerUpdateSelectionMessage(uint32 playerId, uint64 spawnedUnitEntityId);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SpawnUnitAction();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SpawnUnitAction();

		/**
		*  @brief
		*    Initialize action with values
		*/
		void init(const OrderInfo& orderInfo, const glm::vec3& targetPosition, Player* player, const OrderInfo::OrderPairList& orderPairList = OrderInfo::OrderPairList(), int index = 1, int maxIndex = 1, const glm::quat* targetRotation = nullptr, uint64 userData = qsf::getUninitialized<uint64>());


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::action::Result updateAction(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void registerUnitWithPersonnelInSpawn();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum State
		{
			STATE_INIT,
			STATE_WAIT_FORSPACE,
			STATE_WAIT_FORORDERTIME,
			STATE_SPAWNUNIT,
			STATE_SPAWNUNIT_2_PART,
			STATE_CLOSEDOORS
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		State						mCurrentState;
		const OrderInfo*			mOrderInfo;			///< Order information, can be a null pointer
		glm::vec3					mTargetPosition;
		boost::optional<glm::quat>  mTargetRotation;
		uint64						mUserData;			///< Optional user data to send in "em5::Messages::EM5_SPAWN_UNIT", "qsf::getUninitialized<uint64>()" if there's no user data
		Player*						mPlayer;			///< Player instance, can be a null pointer
		qsf::Time					mOrderedTimeStamp;	///< The time when the ordering was happen
		OrderInfo::OrderPairList	mOrderPairList;
		int							mIndex;
		int							mMaxIndex;
		qsf::WeakPtr<qsf::Entity>	mNewSpawnedVehicle;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::SpawnUnitAction)
