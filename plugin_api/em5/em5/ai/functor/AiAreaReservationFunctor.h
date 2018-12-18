// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/navigation/reservation/ReservationContainer.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Clock;
	class MovableComponent;
	namespace ai
	{
		class NavigationComponent;
	}
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
	*    AI area reservation functor
	*
	*  @remarks
	*    This class's operator() will be called by the qsf::ai::ReservationSystem,
	*    to query for specific reaction on scheduling problems while reserving world areas.
	*    Ranks two colliding reservations on "who has to brake harder" and decides upon this which
	*    reservation is kept/inserted.
	*
	*  @todo
	*    - TODO(bs) This is not a complete implementation yet, vehicle/mover types etc still need to be considered.
	*/
	class AiAreaReservationFunctor : public qsf::ai::ReservationConflictResolver
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		AiAreaReservationFunctor();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ai::ReservationConflictResolver methods ]
	//[-------------------------------------------------------]
	public:
		virtual Result canInsertAfterResolve(const std::vector<ContainerType::const_iterator>& collisions, const qsf::ai::Reservation& reservation, const qsf::ai::AreaConfiguration& reservationId,
			std::vector<bool>& deleteFlags, uint32 flags) override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static bool areReservationsMutuallyExclusive(const qsf::ai::Reservation& lhs, const qsf::ai::Reservation& rhs, const qsf::MovableComponent& lhsMovable, const qsf::MovableComponent* rhsMovable, uint32 flags);

		// Higher means more critical here, should only return positive values from zero to +infinity
		static float calculateCriticality(qsf::Time now, qsf::Time reservationBegin, const qsf::MovableComponent* movable, const qsf::ai::NavigationComponent& navi);
		// Returns whether the reservation is for the area the entity is already inside
		static bool isReservationForCurrentArea(const qsf::ai::NavigationComponent& navi, const qsf::ai::AreaConfiguration& reservation);
		static bool isCar(uint32 moverType);
		static bool isCivil(uint32 moverType);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const qsf::Clock& mClock;	///< Direct reference to the game simulation clock


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
