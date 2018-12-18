// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/logic/gamelogic/GameLogic.h>


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
	*    Reserve logic
	*/
	class EM5_API_EXPORT ReserveLogic : public qsf::GameLogic
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveLogic" game logic type ID

		enum ReservePriority
		{
			STANDARD = 100,		///< Almost all reservations have this level
			LOW = 50,
			MOVETO = 10,		///< Reservate but on the way to target, no interacting happens yet
			NO_UNIT = 5			///< Reservation from a non unit
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create the given reservation type at the target and set the reservation ID
		*/
		template<typename T>
		static void createReservation(qsf::Entity& targetEntity, uint64 reservationId);

		/**
		*  @brief
		*    Check if entity has an reservation and if it reserve the given id
		*
		*  @return
		*    "true" if no reservation is active or the current reservation is the given id
		*/
		static bool checkReservation(qsf::Entity& targetEntity, uint64 reservationId, ReservePriority priority = ReservePriority::STANDARD);

		/**
		*  @brief
		*    Checks if the target reservation logic matches the given reservation id, in case "false" delete the reservation logic of the target entity
		*/
		static void deleteOwnReservation(qsf::Entity& targetEntity, uint64 reservationId);

		/**
		*  @brief
		*    Checks if the target reservation logic matches the given reservation id, in case "false" delete the reservation logic of the target entity
		*/
		static void deleteOwnReservation(const qsf::WeakPtr<qsf::Entity>& targetEntity, uint64 reservationId);

		/**
		*  @brief
		*    Checks if the target reservation logic matches the given reservation id, in case "false" delete the reservation logic of the target entity
		*/
		static void deleteOwnReservation(qsf::Map& map, uint64 targetEntityId, uint64 reservationId);

		static void createReservationByTypeId(qsf::Entity& targetEntity, uint64 reservationId, uint32 typeId);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit ReserveLogic(uint32 gamelogicTypeId);
		uint64 getReservationId();
		void setReservationId(uint64 reservationId);
		void deleteLogic();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		uint64 mReservationId;	///< ID of the entity who reserved this
		ReservePriority	mReservePriority;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	};


	/**
	*  @brief
	*    Reserve paramedic logic
	*/
	class EM5_API_EXPORT ReserveParamedicLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveParamedicLogic" game logic type ID

	public:
		ReserveParamedicLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve doctor logic
	*/
	class EM5_API_EXPORT ReserveDoctorLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveDoctorLogic" game logic type ID

	public:
		ReserveDoctorLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve cut road vehicle logic
	*/
	class EM5_API_EXPORT ReserveCutRoadVehicleLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveCutRoadVehicleLogic" game logic type ID

	public:
		ReserveCutRoadVehicleLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve enter vehicle logic
	*/
	class EM5_API_EXPORT ReserveEnterVehicleLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveEnterVehicleLogic" game logic type ID

	public:
		ReserveEnterVehicleLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve guide person logic
	*/
	class EM5_API_EXPORT ReserveGuidePersonLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveGuidePersonLogic" game logic type ID

	public:
		ReserveGuidePersonLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve decontaminate person logic
	*/
	class EM5_API_EXPORT ReserveDecontaminatePersonLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveDecontaminatePersonLogic" game logic type ID

	public:
		ReserveDecontaminatePersonLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve diving spot logic
	*/
	class EM5_API_EXPORT ReserveDivingSpotLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveDivingSpotLogic" game logic type ID

	public:
		ReserveDivingSpotLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve wait for finish
	*/
	class EM5_API_EXPORT ReserveWaitForFinishLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveWaitForFinishLogic" game logic type ID

	public:
		ReserveWaitForFinishLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve dlk rescue logic
	*/
	class EM5_API_EXPORT ReserveDLKRescueLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveDLKRescueLogic" game logic type ID

	public:
		ReserveDLKRescueLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve rescue dog logic
	*/
	class EM5_API_EXPORT ReserveRescueDogLogic : public ReserveLogic
	{
	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveRescueDogLogic" game logic type ID

	public:
		ReserveRescueDogLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve gangster logic, mainly used for gangster fights
	*/
	class EM5_API_EXPORT ReserveGangsterLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveGangsterLogic" game logic type ID

	public:
		ReserveGangsterLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve arrest gangster logic
	*/
	class EM5_API_EXPORT ReserveArrestGangsterLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveArrestGangsterLogic" game logic type ID

	public:
		ReserveArrestGangsterLogic();

		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};

	/**
	*  @brief
	*    Reserve close combat logic
	*/
	class EM5_API_EXPORT ReserveCloseCombatLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveCloseCombatLogic" game logic type ID

	public:
		ReserveCloseCombatLogic();

		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};

	/**
	*  @brief
	*    Reserve attacked by gangster logic
	*/
	class ReserveAttackedByGangsterLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveAttackedByGangsterLogic" game logic type ID

	public:
		ReserveAttackedByGangsterLogic();

		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};

	/**
	*  @brief
	*    Reserve pulling logic
	*/
	class EM5_API_EXPORT ReservePullingLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReservePullingLogic" game logic type ID

	public:
		ReservePullingLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Move to target logic
	*/
	class EM5_API_EXPORT ReserveMoveToTargetLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveMoveToTargetLogic" game logic type ID

	public:
		ReserveMoveToTargetLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve rabid dog logic
	*/
	class EM5_API_EXPORT ReserveRabidDogLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveRabidDogLogic" game logic type ID

	public:
		ReserveRabidDogLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve repair or uselogic
	*/
	class EM5_API_EXPORT ReserveRepairOrUseLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveRepairOrUseLogic" game logic type ID

	public:
		ReserveRepairOrUseLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve Cuttree or hedge logic
	*/
	class EM5_API_EXPORT ReserveCutTreeLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveCutTreeLogic" game logic type ID

	public:
		ReserveCutTreeLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve cars for the towcar logic
	*/
	class EM5_API_EXPORT ReserveTowcarLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveTowcarLogic" game logic type ID

	public:
		ReserveTowcarLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


	/**
	*  @brief
	*    Reserve cars for the heavy crane logic
	*/
	class EM5_API_EXPORT ReserveHeavyCraneLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::ReserveHeavyCraneLogic" game logic type ID

	public:
		ReserveHeavyCraneLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};

	/**
	*  @brief
	*    Reserve speak to person logic
	*/
	class EM5_API_EXPORT SpeakToPersonLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::SpeakToPersonLogic" game logic type ID

	public:
		SpeakToPersonLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};

	/**
	*  @brief
	*    Reserve rescue witch logic
	*/
	class EM5_API_EXPORT RescueWitchLogic : public ReserveLogic
	{

	public:
		static const uint32 GAMELOGIC_TYPE_ID;	// "em5::RescueWitchLogic" game logic type ID

	public:
		RescueWitchLogic();

	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveParamedicLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveDoctorLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveCutRoadVehicleLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveEnterVehicleLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveGuidePersonLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveDecontaminatePersonLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveDivingSpotLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveWaitForFinishLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveDLKRescueLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveRescueDogLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveGangsterLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveArrestGangsterLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveCloseCombatLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveAttackedByGangsterLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReservePullingLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveMoveToTargetLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveRabidDogLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveRepairOrUseLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveCutTreeLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveTowcarLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::ReserveHeavyCraneLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::SpeakToPersonLogic)
QSF_CAMP_TYPE_NONCOPYABLE(em5::RescueWitchLogic)


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/logic/local/ReserveLogic-inl.h"
