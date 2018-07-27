// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/SpawnPointComponent.h"

#include <qsf/reflection/type/CampGlmVec2.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/** EMERGENCY 5 Inhabitant Spawn component.
	* Attached to buildings that are supposed to spawn a civilian entity at a specific day-time.
	* This component takes care that the desired civilian is spawned at the time specified.
	* It also ensures that the spawned entity has an InhabitantComponent attached.
	* The spawned entity is commanded to go to the destination entity specified.
	*/
	class EM5_API_EXPORT InhabitantSpawnComponent : public SpawnPointComponent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;			///< "em5::InhabitantSpawnComponent" unique component ID
		static const uint32 PERSON;					///< "Person" unique class property ID inside the class
		static const uint32 DESTINATION_ENTITY_ID;	///< "DestinationEntityId" unique class property ID inside the class
		static const uint32 TIME_BEGIN;				///< "TimeBegin" unique class property ID inside the class
		static const uint32 TIME_END;				///< "TimeEnd" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit InhabitantSpawnComponent(qsf::Prototype* prototype);
		virtual ~InhabitantSpawnComponent();

		virtual void serialize(qsf::BinarySerializer& serializer) override;

		// Returns true if the inhabitant person is allowed to be spawned.
		bool allowSpawn(int currentMinuteOfDay) const;

		// Spawns the inhabitant if it has not been spawned already. Returns true if spawning succeeded.
		bool spawnInhabitant();

		// Get / set the person to spawn.
		//@{
		const std::string& getPerson() const;
		void setPerson(const std::string& personName);
		//@}

		// Get / set the entity id of the destination the inhabitant is supposed to reach.
		//@{
		uint64 getDestinationEntityId() const;
		void setDestinationEntityId(uint64 entityId);
		//@}

		// The starting-time at which spawning is desired, x = hour in [0, 23]h range, y = minute in [0, 59]m range
		//@{
		glm::vec2 getTimeBegin() const;
		void setTimeBegin(const glm::vec2& time);
		//@}

		// The ending-time at which spawning is desired, x = hour in [0, 23]h range, y = minute in [0, 59]m range
		//@{
		glm::vec2 getTimeEnd() const;
		void setTimeEnd(const glm::vec2& time);
		//@}

		// Checks if the floatTime is valid (x = hour in [0, 23]h range, y = minute in [0, 59]m range)
		static bool isValidFloatTime(const glm::vec2& floatTime);

		// Convert a glm::vec2 time to the corresponding minute-of-day or vice versa.
		//@{
		static int floatTimeToToMinuteOfDay(const glm::vec2& floatTime);
		static glm::vec2 minuteOfDayToFloatTime(int minuteOfDay);
		//@}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Calculates the real spawn time mTime to be a random value between mTimeBegin and mTimeEnd.
		void updateSpawnTime();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		uint64 mDestinationEntityId;			///< The entity id of the destination the inhabitant is supposed to reach.
		int mTimeBegin;							///< The start of the time interval at which to spawn, converted to minute-of-day.
		int mTimeEnd;							///< The end of the time interval at which to spawn, converted to minute-of-day.

		int mTime;								///< The real time at which to spawn, randomly chosen between mTimeBegin and mTimeEnd, in minute-of-day.
		qsf::WeakPtr<qsf::Entity> mInhabitant;	///< Pointer to the spawned inhabitant, if spawned and not despawend already.


	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/spawnpoint/InhabitantSpawnComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::InhabitantSpawnComponent)
