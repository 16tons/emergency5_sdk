// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    EMERGENCY 5 freeplay event "medical emergency in building"
	*/
	class EM5_API_EXPORT MedicalInBuildingEvent : public FreeplayEvent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class MedicalInBuldingEventFactory;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 FREEPLAY_EVENT_ID;		///< "em5::MedicalInBuildingEvent" unique freeplay event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		MedicalInBuildingEvent();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MedicalInBuildingEvent();

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

		void setAmbientLayerName(const std::string& layerName);

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
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(FailAmountInfectedPersons, int, mFailAmountInfectedPersons)

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(SpawnFromConnectedSpawnpools, bool, mSpawnFromConnectedSpawnpools)

		/**
		*   @brief
		*    Get or set if the door is blocked and need to be open with an axe
		*/
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(BlockedDoor, bool, mBlockedDoor)


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
		void startHintObservers(const qsf::Entity& targetEntity);

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

		void spawnPersonsFromConnectedSpawnPoints(qsf::Entity& buildingEntity);

		int getNumberInfectedPersons();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint64		mTargetEntityId;				///< ID of the inflamed building entity
		uint32		mDeadPersonsToFail;				///< Number of persons that have to die until the player has failed this event
		bool		mPersonsInside;					///< Flag if persons are inside the burning building
		bool		mPersonsAtWindow;				///< Flag if a person is at the window of the burning building
		qsf::Time	mTrappedPersonsRescueTime;		///< Time to rescue persons from the building. 300 sec.
		uint32		mTrappedHealthyPersonsMin;		///< Minimum number of trapped and healthy persons in the building (only relevant if it's a fire event with person damage). Default: 1 person
		uint32		mTrappedHealthyPersonsMax;		///< Maximum number of trapped and healthy persons in the building (only relevant if it's a fire event with person damage). Default: 3 persons
		uint32		mTrappedInjuredPersonsMin;		///< Minimum number of trapped and injured persons in the building (only relevant if it's a fire event with person damage). Default: 1 person
		uint32		mTrappedInjuredPersonsMax;		///< Maximum number of trapped and injured persons in the building (only relevant if it's a fire event with person damage). Default: 3 persons
		bool		mTrappedPersonWithPlague;		///< Injure the trapped Person with plague
		bool		mBlockedDoor;					///< Do we need to open the door with the axe first or not
		bool		mSpawnFromConnectedSpawnpools;	///< "true" if connected spawnpoints are used for spawning injured persons
		uint32		mTrappedPersons;				///< Actual number of trapped (injured and healthy) persons in the building (only relevant if it's a fire event with person damage)
		uint32		mTrappedInjuredPersons;			///< Actual number of trapped and injured persons in the building (only relevant if it's a fire event with person damage)
		std::string mAmbientLayerName;				///< The name of the ambient layer, if set the layer is enabled for this event

		int			mFailAmountInfectedPersons;		///< The number of maximal infected persons

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
QSF_CAMP_TYPE_NONCOPYABLE(em5::MedicalInBuildingEvent)
