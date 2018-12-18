// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"
#include "em5/freeplay/eventspread/EventSpreadReason.h"

#include <qsf/component/Component.h>
#include <qsf/math/EnumBitmask.h>


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
	*    EMERGENCY 5 event ID component class
	*/
	class EM5_API_EXPORT EventIdComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Objective;			// To access and modify the mPlayerInteractedWith list


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::EventIdComponent" unique component ID


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool addToEvent(qsf::Entity& entity, FreeplayEvent& freeplayEvent, eventspreadreason::Reason reason);

		/**
		*  @brief
		*    Tries to lock the event object to the team
		*
		*  @param[in] targetEntity
		*    The entity for which the event object should be acquired
		*  @param[in] unitEntity
		*	 The unit entity which wants to acquire the lock for the team
		*
		*  @return
		*    "true" if the lock was acquired, "false" otherwise
		*
		*  @note
		*    The locking is a kind of "reference counting", when multiple units from the same team acquires the lock, then the lock is then unlocked, when all units release the lock
		*/
		static bool tryLockForTeam(const qsf::Entity& targetEntity, const qsf::Entity& unitEntity);

		/**
		*  @brief
		*    Releases a team lock
		*
		*  @param[in] targetEntity
		*    The entity for which the event object should be released
		*  @param[in] unitEntity
		*	 The unit entity which wants to release the lock for the team
		*
		*  @note
		*    The locking is a kind of "reference counting", when multiple units from the same team acquires the lock, then the lock is then unlocked, when all units release the lock
		*/
		static void	releaseTeamLock(const qsf::Entity& targetEntity, const qsf::Entity& unitEntity);

		/**
		*  @brief
		*    Releases a team lock
		*
		*  @param[in] targetEntityId
		*    The entity id for which the event object should be released
		*  @param[in] unitEntity
		*	 The unit entity which wants to release the lock for the team
		*
		*  @note
		*    The locking is a kind of "reference counting", when multiple units from the same team acquires the lock, then the lock is then unlocked, when all units release the lock
		*/
		static void	releaseTeamLock(uint64 targetEntityId, qsf::Entity& unitEntity);

		/**
		*  @brief
		*    Check if the event object is not locked for the team
		*
		*  @param[in] targetEntity
		*    The entity for which the event object lock should be checked
		*  @param[in] unitEntity
		*	 The unit entity which wants to check the lock
		*
		*  @return
		*    "true" if the lock is free for the team, "false" otherwise
		*
		*  @note
		*    The locking is a kind of "reference counting", when multiple units from the same team acquires the lock, then the lock is then unlocked, when all units release the lock
		*    This check returns also true when the event object is locked, but both units are from the same team
		*/
		static bool	isUnlockedForTeam(const qsf::Entity& targetEntity, const qsf::Entity& unitEntity);

		/**
		*  @brief
		*    Check if the event object is not locked for the team
		*
		*  @param[in] targetEntity
		*    The entity id for which the event object lock should be checked
		*  @param[in] teamId
		*	 The team id to check
		*
		*  @return
		*    "true" if the lock is free for the team, "false" otherwise
		*
		*  @note
		*    The locking is a kind of "reference counting", when multiple units from the same team acquires the lock, then the lock is then unlocked, when all units release the lock
		*    This check returns also true when the event object is locked, but the asking unit is in the team which holds the lock
		*/
		static bool	isUnlockedForTeam(const qsf::Entity& targetEntity, uint16 teamId);

		/**
		*  @brief
		*    Registers the player at the event object as "has interacted with"
		*
		*  @param[in] targetEntity
		*    The entity for which the registration of the player should be done
		*  @param[in] unitEntity
		*	 The unit entity which holds the information about the player to register
		*
		*  @note
		*    This information is used to know which player has interacted with the event object when the per objective points are distributed
		*/
		static void registerPlayer(const qsf::Entity& targetEntity, const qsf::Entity& unitEntity);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit EventIdComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~EventIdComponent();

		/**
		*  @brief
		*    Get event ID
		*/
		uint32 getEventId() const;

		/**
		*  @brief
		*    Return the registered event (could be nullptr!)
		*/
		inline FreeplayEvent* getFreeplayEvent() const;

		/**
		*  @brief
		*    Set the event ID and register at this event; once the ID is set it won't be changed
		*
		*  @param[in] freeplayEvent
		*    Event ID to set
		*  @param[in] eventSpreadReason
		*	 Event spreading reason; this is usually an enum value
		*
		*  @return
		*    "true" if this entity is already part of the event or the event is succesfully created, "false" otherwise
		*/
		bool setEvent(FreeplayEvent& freeplayEvent, eventspreadreason::Reason eventSpreadReason);

		/**
		*  @brief
		*    Set the event ID once the ID is set it won't be changed
		*
		*  @param[in] freeplayEvent
		*    Event ID to set
		*
		*  @note
		*	This method sets only the event id. This method is only used for multiplay on the multiplay client side
		*/
		void setEvent(FreeplayEvent& freeplayEvent);


		//[-------------------------------------------------------]
		//[ Object locking                                        ]
		//[-------------------------------------------------------]
		void lockForTeam(uint64 unitEntityId, uint16 teamId);
		void releaseTeamLock(uint64 unitEntityId, uint16 teamId);
		bool isUnlockedForTeam(uint16 teamId);
		uint16 getCurrentTeamId() const;
		// This only needed for multiplay on client side
		void setCurrentTeamId(uint16 teamId);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void serialize(qsf::BinarySerializer& serializer) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void registerPlayerAsInteractedWithMe(uint32 playerIndex);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::WeakPtr<FreeplayEvent>	mFreeplayEvent;
		uint16						mCurrentTeamId;			///< Contains the team ID, whose units are currently interacting with the entity
		std::vector<uint64>			mCurrentPlayerRefCount;	///< "Reference count" for the current player index, to avoid unsetting mCurrentPlayerIndex when two units of the same player is interacting with the object and one stops (Does an unregister)
		uint8						mPlayerInteractedWithFlags;

		typedef qsf::EnumBitmask<uint16, eventspreadreason::Reason> EventSpreadReasonBitmask;
		EventSpreadReasonBitmask mEventSpreadReasons;


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/event/EventIdComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::EventIdComponent)
