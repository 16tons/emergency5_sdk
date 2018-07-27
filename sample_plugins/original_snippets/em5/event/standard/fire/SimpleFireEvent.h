// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/freeplay/event/FreeplayEvent.h"

#include <qsf/message/MessageProxy.h>
#include <qsf/base/PropertyHelper.h>


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
	*    EMERGENCY 5 freeplay event "simple fire"
	*/
	class EM5_API_EXPORT SimpleFireEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class BuildingFireEventFactory;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::SimpleFireEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SimpleFireEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SimpleFireEvent();

		/**
		*  @brief
		*    Get the ID of the target entity
		*/
		uint64 getTargetEntityId() const;

		/**
		*  @brief
		*    Set the target entity
		*/
		void setTargetEntity(const qsf::Entity& entity);

		/**
		*  @brief
		*    Get number persons that have to die so that the event fails
		*/
		uint32 getDeadPersonsToFail() const;

		/**
		*  @brief
		*    Set number persons that have to die so that the event fails
		*/
		void setDeadPersonsToFail(uint32 personsToFail);

		/**
		*  @brief
		*    Get or set if the building explodes
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(Explosion, bool, mExplosion)

		/**
		*  @brief
		*    Get or set if the building explodes
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(ExplosionRadius, float, mExplosionRadius)

		/**
		*   @brief
		*    Get or set if the building could be an industrial plant
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(IndustrialPlant, bool, mIndustrialPlant)

		/**
		*   @brief
		*    Get or set if the building has defect eboxes which block extinguishing
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(DefectEbox, bool, mDefectEbox)

		/**
		*   @brief
		*    Get or set if there are persons inside the building
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonsInside, bool, mPersonsInside)

		/**
		*   @brief
		*    Get or set if a person is at the window of the burning building
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PersonsAtWindow, bool, mPersonsAtWindow)

		/**
		*   @brief
		*    Get or set the time to rescue trapped persons
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedPersonsRescueTime, qsf::Time, mTrappedPersonsRescueTime)

		/**
		*   @brief
		*    Get or set the number of trapped persons
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedHealthyPersonsMin, uint32, mTrappedHealthyPersonsMin)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedHealthyPersonsMax, uint32, mTrappedHealthyPersonsMax)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedInjuredPersonsMin, uint32, mTrappedInjuredPersonsMin)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedInjuredPersonsMax, uint32, mTrappedInjuredPersonsMax)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(TrappedPersonWithPlague, bool, mTrappedPersonWithPlague)

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SpawnFromConnectedSpawnpools, bool, mSpawnFromConnectedSpawnpools)

		/**
		*   @brief
		*    Get or set if the door is blocked and need to be open with an axe
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BlockedDoor, bool, mBlockedDoor)

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(InjuredSquadToFail, uint32, mInjuredSquadToFail)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BurningObjectsToFail, uint32, mBurningObjectsToFail)

		void setAmbientLayerName(const std::string& ambientLayerName);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		virtual void onRun() override;
		virtual bool onFailure(EventResult& eventResult) override;
		virtual void updateFreeplayEvent(const qsf::Time& timePassed) override;
		virtual bool addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason) override;
		virtual void hintCallback(Observer& hintObserver) override;
		virtual const qsf::Entity* getFocusEntity() override;
		virtual bool checkEventConfiguration() override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void startObjectives(const qsf::Entity& targetEntity);
		void startHintObservers(const qsf::Entity& targetEntity);

		/**
		*  @brief
		*    Execute a normal fire event
		*/
		void executeNormalFire(qsf::Entity& entity);

		/**
		*  @brief
		*    Execute a fire event with a peron at the window
		*/
		void executeFireWithPersonAtWindow(qsf::Entity& entity);

		/**
		*  @brief
		*    Execute a fire event with persons inside the building
		*/
		void executeFireWithPersonsInside(qsf::Entity& entity);

		/**
		*  @brief
		*    Execute a building explosion event
		*/
		void executeExplosion(qsf::Entity& entity);

		/**
		*  @brief
		*    Execute a industrial plant fire event
		*/
		void executeIndustrialPlantFire(qsf::Entity& entity);

		void onContaminationStartSpreading(const qsf::MessageParameters& parameters);
		void onNeedContaminated(const qsf::MessageParameters& parameters);
		void onPersonSaved(const qsf::MessageParameters& parameters);
		bool injureSavedPerson(FreeplayEvent& freeplayEvent, qsf::Entity& targetEntity);
		void spawnInContaminationArea(qsf::Entity& buildingEntity);
		void spawnPersonsFromConnectedSpawnPoints(qsf::Entity& buildingEntity);

	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		uint64		mTargetEntityId;				///< ID of the inflamed building entity


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32		mDeadPersonsToFail;				///< Number of persons that have to die until the player has failed this event
		uint32		mInjuredSquadToFail;			///< Number of units that have to get injured until the event is failed
		uint32		mBurningObjectsToFail;			///< Number of objects that have to burning until the event is failed
		bool		mExplosion;						///< Does the building explode at the event start
		bool		mIndustrialPlant;				///< Is the target a industrial plant?
		bool		mDefectEbox;					///< Do we have defect eboxes which block fire extinguishing
		float		mExplosionRadius;				///< The radius of the explosion
		uint64		mExplosionId;					///< ID of the explosion particle entity
		bool		mPersonsInside;					///< Flag if persons are inside the burning building
		bool		mPersonsAtWindow;				///< Flag if a person is at the window of the burning building
		qsf::Time	mTrappedPersonsRescueTime;		///< Time to rescue persons from the building. 300 sec.
		uint32		mTrappedHealthyPersonsMin;		///< Minimum number of trapped and healthy persons in the building (only relevant if it's a fire event with person damage). Default: 1 person
		uint32		mTrappedHealthyPersonsMax;		///< Maximum number of trapped and healthy persons in the building (only relevant if it's a fire event with person damage). Default: 3 persons
		uint32		mTrappedInjuredPersonsMin;		///< Minimum number of trapped and injured p persons in the building (only relevant if it's a fire event with person damage). Default: 1 person
		uint32		mTrappedInjuredPersonsMax;		///< Maximum number of trapped and injured ppersons in the building (only relevant if it's a fire event with person damage). Default: 3 persons
		bool		mTrappedPersonWithPlague;		///< Injure the trapped person with plague
		bool		mBlockedDoor;					///< Do we need to open the door with the axe first or not
		bool		mSpawnFromConnectedSpawnpools;	///< "true" if connected spawnpoints are used for spawning injured persons
		uint32		mTrappedPersons;				///< Actual number of trapped (injured and healthy) persons in the building (only relevant if it's a fire event with person damage)
		uint32		mTrappedInjuredPersons;			///< Actual number of trapped and injured persons in the building (only relevant if it's a fire event with person damage)
		std::string mAmbientLayerName;				///< The name of the ambient layer, if set the layer is enabled for this event

		qsf::Time	mHint01WaitTime;				///< 60 sec.
		std::vector<uint64> mVictimSpawnPoints;
		qsf::MessageProxy mMessageProxyStoppedBurning;
		qsf::MessageProxy mMessageProxyContaminationStart;		///< Get a message when the industry contamination has started
		qsf::MessageProxy mMessageProxyNeedContaminatedTime;	///< Time to check if we have any contaminated persons


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
QSF_CAMP_TYPE_NONCOPYABLE(em5::SimpleFireEvent)
