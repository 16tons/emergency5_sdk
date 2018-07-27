// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/UnitSpawnPointComponent.h"
#include "em5/game/units/OrderInfo.h"

#include <qsf_game/command/mode/CommandMode.h>

#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/time/Time.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Player;
	class CommandContext;
	class SpawnUnitAction;
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
	*    Order unit command mode
	*
	*  @remarks
	*    This command mode lets the user select a position where to send an ordered unit to
	*/
	class EM5_API_EXPORT OrderUnitCommandMode : public qsf::game::CommandMode
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::OrderUnitCommandMode" unique command pluginable ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static SpawnUnitAction* orderUnit(const OrderInfo& orderInfo, Player& player, const OrderInfo::OrderPairList& orderPairList, const glm::vec3& targetPosition, const glm::quat* targetRotation = nullptr);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		OrderUnitCommandMode();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OrderUnitCommandMode();

		/**
		*  @brief
		*    Fill class with all necessary variables
		*
		*  @param[in] orderInfo
		*    The order information of the type of unit to order; instance must stay valid as long as this instance exists
		*  @param[in] numberToOrder
		*    Number of vehicles to order, the default value is one
		*/
		void initialize(const OrderInfo& orderInfo, uint32 numberToOrder = 1, OrderInfo::OrderPairList orderPairList = OrderInfo::OrderPairList());

		/**
		*  @brief
		*    Sets the entity instance which defines the area in which an ordering should only be possible
		*
		*  @param[in] orderOnlyInThisAreaEntity
		*    The entity which defines the order only area, can be a nullptr to disable this restriction
		*/
		void setOrderOnlyInThisAreaEntity(qsf::Entity* orderOnlyInThisAreaEntity);

		const OrderInfo* getOrderInfo() const;
		uint32 getNumberToOrder() const;
		void setNumberToOrder(uint32 numberToOrder);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::game::CommandMode methods         ]
	//[-------------------------------------------------------]
	public:
		virtual void startup() override;
		virtual void shutdown() override;
		virtual bool executeCommandContext(qsf::game::CommandContext& commandContext, const EntityIdSet& callerIds) override;
		virtual void updateCommandMode(const qsf::Clock& clock) override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static UnitSpawnPointComponent* findUnitSpawnPoint(const OrderInfo& orderInfo, UnitSpawnPointComponent::UnitSpawnPointType type, const glm::vec3& targetPosition);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool allowOrderingUnderMousePosition();
		bool getTargetUnderMouse(glm::vec3& targetPosition, qsf::Entity*& targetEntity);
		bool allowOrderingAtTarget(const glm::vec3& targetPosition, const qsf::Entity* targetEntity);
		void triggerUnitSpawn(const UnitSpawnPointComponent& unitSpawnPointComponent, uint32 index, CommandContext& commandContext);
		void showAllHarborIcons(bool show);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const uint32 ACTION_PRIORITY;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const OrderInfo*		 mOrderInfo;					///< Order information, can be a null pointer, don't destroy the instance
		qsf::Entity*			 mOrderOnlyInThisAreaEntity;	///< When set the ordering of a unit is only possible within the area of that entity
		uint32					 mNumberToOrder;
		OrderInfo::OrderPairList mOrderPairList;
		bool					 mPlayAudioForOrderUnit;
		qsf::Time				 mElapsedWaitTime;

		// Debug
		qsf::DebugDrawProxy		 mDebugDrawProxy;


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::OrderUnitCommandMode)
