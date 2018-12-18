// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/reflection/CampClass.h>

#include <boost/noncopyable.hpp>
#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class BinarySerializer;
	class Entity;
	class Map;
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
	*    EMERGENCY 5 objective class
	*
	*  @remarks
	*    An objective is a task or "quest" the player has to accomplish as part of an event.
	*    There is also the objective type "fail condition" to cover not only conditions for successfully solving an event,
	*    but also for failure.
	*/
	class EM5_API_EXPORT Objective : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ObjectiveHelper;
		friend class ObjectiveManager;	// Writes the objective unique ID


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_set<uint64> EntityIdSet;
		typedef std::array<uint16, 4> PlayerCountMap;

		/** Objective type */
		enum ObjectiveType
		{
			OBJECTIVETYPE_REQUIRED,			///< Objective is required for success
			OBJECTIVETYPE_FAILCONDITION,	///< Objective is a fail condition, and will lead to a failure if accomplished
			OBJECTIVETYPE_OPTIONAL,			///< Objective is optional (irrelevant for success) and still open
			OBJECTIVETYPE_OPTIONAL_FAILED	///< Objective is optional (irrelevant for success) and failed
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static Objective* getObjectiveById(uint32 id);


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		Objective(uint32 typeId, ObjectiveType type, uint32 eventId);

		/**
		*  @brief
		*    Constructor for serialization
		*/
		explicit Objective(qsf::BinarySerializer& serializer);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~Objective();

		inline uint32 getId() const;
		inline uint32 getTypeId() const;

		/**
		*  @brief
		*    Get freeplay event ID
		*/
		inline uint32 getFreeplayEventId();

		inline ObjectiveType getObjectiveType() const;
		inline void setObjectiveType(ObjectiveType type);

		/**
		*  @brief
		*    Get the current objective text of this objective
		*/
		inline const std::string& getText() const;

		/**
		*  @brief
		*    Set the objective text
		*/
		inline void setText(const std::string& text);

		inline uint32 getCurrentNumber() const;
		void setCurrentNumber(uint32 number);

		inline uint32 getNeededNumber() const;
		void setNeededNumber(uint32 number);

		bool isCurrentEntity(qsf::Entity& entity) const;
		bool isNeededEntity(qsf::Entity& entity) const;

		inline const EntityIdSet& getCurrentEntityIdSet() const;
		inline const EntityIdSet& getNeededEntityIdSet() const;

		/**
		*  @brief
		*    Increase the current number and delete entity ID
		*
		*  @param[in] entity
		*    This is the entity that has to be deleted; if it is a "nullptr" just increase current number
		*/
		void increaseCurrentNumber(qsf::Entity* entity = nullptr);

		/**
		*  @brief
		*    Decrease the current number and add entity ID
		*
		*  @param[in] entity
		*    This is the entity that has to be added; if it is a "nullptr" just decrease current number
		*/
		void decreaseCurrentNumber(qsf::Entity* entity = nullptr);

		/**
		*  @brief
		*    Increase the needed number and add entity ID
		*
		*  @param[in] entity
		*    This is the entity that has to be added; if it is a "nullptr" just increase needed number
		*/
		void increaseNeededNumber(qsf::Entity* entity = nullptr);

		/**
		*  @brief
		*    Decrease the needed number and remove entity ID
		*
		*  @param[in] entity
		*    This is the entity that has to be removed; if it is a "nullptr" just decrease needed number
		*/
		void decreaseNeededNumber(qsf::Entity* entity = nullptr);

		inline bool checkAccomplished() const;
		void setAccomplished();

		/**
		*  @brief
		*    Get random entity ID of current entities, or qsf::getUninitialized<uint64>() if there is none
		*/
		uint64 getRandomCurrentEntityId() const;

		/**
		*  @brief
		*    Get random entity ID of needed entities, or qsf::getUninitialized<uint64>() if there is none
		*/
		uint64 getRandomNeededEntityId() const;

		inline bool isHidden() const  { return mHidden; }
		inline void setHidden(bool hidden)  { mHidden = hidden; }

		inline uint32 getOrderPriority() const  { return mOrderPriority; }
		inline void setOrderPriority(uint32 orderPriority)  { mOrderPriority = orderPriority; }

		/**
		*  @brief
		*    Check if the objective is accomplished and inform the freeplay event about this (for objective individual point calculation)
		*/
		void updateObjectiveProcess();

		inline const PlayerCountMap& getPlayerCountMap() const;

		// Error handling
		void fulfillNonexistingNeededEntities(qsf::Map& map);

		//[-------------------------------------------------------]
		//[ Serialize                                             ]
		//[-------------------------------------------------------]
		void serialize(qsf::BinarySerializer& serializer);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		bool addCurrentEntity(uint64 entityId);
		bool removeCurrentEntity(uint64 entityId);
		bool addNeededEntity(uint64 entityId);
		bool removeNeededEntity(uint64 entityId);
		void emitEventChangeMessage();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32			mUniqueId;			///< Unique objective ID
		uint32			mTypeId;			///< Type ID of this objective
		ObjectiveType	mObjectiveType;		///< Type of this objective or fail condition
		bool			mHidden;			///< "true" if hidden; hidden objectives count for success/failure, but are not shown to the user
		std::string		mText;				///< The text of the objective that is to be shown; in case of fail conditions, this is just internally for debugging
		uint32			mCurrentNumber;		///< Current number for non-boolean objectives
		uint32			mNeededNumber;		///< Needed number for non-boolean objectives
		EntityIdSet		mCurrentEntities;	///< Set of entity IDs that are affected by this objective as current entities
		EntityIdSet		mNeededEntities;	///< Set of entity IDs that are affected by this objective as needed entities (may not contain all, especially if the entities are not known)
		uint32			mFreeplayEventId;	///< The ID of the freeplay event that this objective belongs to
		uint32			mOrderPriority;		///< Ordering priority value for sorted output; objectives with lower values will be displayed first
		PlayerCountMap	mPlayerCounter;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/freeplay/objective/Objective-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::Objective)
