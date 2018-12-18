// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/event/standard/fire/ComplexFireEvent.h"
#include "em5/event/EventHelper.h"
#include "em5/game/player/PlayerManager.h"
#include "em5/game/units/UnitPool.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/HintHelper.h"
#include "em5/logic/observer/hint/HintMessageGeneralObserver.h"
#include "em5/logic/observer/PersonDiedObserver.h"
#include "em5/plugin/Messages.h"
#include "em5/specs/ActionRangeSpecsGroup.h"
#include "em5/map/EntityHelper.h"

#include <qsf/renderer/component/RendererComponent.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Local definitions                                     ]
	//[-------------------------------------------------------]
	// Event tags
	const std::string EVENT_TAG_FOREST_FIRE		= "FOREST_FIRE";
	const std::string EVENT_TAG_FOREST_FIRE_P	= "FOREST_FIRE_P";

	// Event names
	const std::string EVENT_NAME_FOREST_FIRE	= "EM5_EVENT_TITLE_08";
	const std::string EVENT_NAME_FOREST_FIRE_P	= "EM5_EVENT_TITLE_09";

	// Supervisor messages
	const std::string SUPERV_OBJ_FORRESTFIRE	= "EM5_EVENT_SUPERV_OBJ_FORRESTFIRE_01_01";	// Without person damage
	const std::string SUPERV_OBJ_FORRESTFIRE_P	= "EM5_EVENT_SUPERV_OBJ_FORRESTFIRE_02_01";	// With person damage

	// Hints
	const std::string USE_FIREFIGHTINGPLANE_HINT_01	= "EM5_EVENT_USE_FIREFIGHTINGPLANE_HINT_01";	// Use firefighting hint 01
	const std::string USE_FIREFIGHTINGPLANE_HINT_02	= "EM5_EVENT_USE_FIREFIGHTINGPLANE_HINT_02";	// Use firefighting hint 02
	const qsf::Time USE_FIREFIGHTINGPLANE_HINT_WAITTIME(qsf::Time::fromSeconds(60.0f));				// Hint wait time for use firefighting plane hint


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 ComplexFireEvent::FREEPLAY_EVENT_ID = qsf::StringHash("em5::ComplexFireEvent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ComplexFireEvent::ComplexFireEvent() :
		mMetaEntityId(qsf::getUninitialized<uint64>()),
		mDeadPersonsToFail(qsf::getUninitialized<uint32>()),
		mPersonDamage(false),
		mPersonMissing(false),
		mAllPersonsFound(false),
		mIconEntity(nullptr)
	{
		// Nothing here
	}

	ComplexFireEvent::~ComplexFireEvent()
	{
		// Nothing here
	}

	uint32 ComplexFireEvent::getDeadPersonsToFail() const
	{
		return mDeadPersonsToFail;
	}

	void ComplexFireEvent::setDeadPersonsToFail(uint32 personsToFail)
	{
		mDeadPersonsToFail = personsToFail;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::FreeplayEvent methods             ]
	//[-------------------------------------------------------]
	bool ComplexFireEvent::onStartup()
	{
		// Event is now running
		setRunning();

		// Done
		return true;
	}

	void ComplexFireEvent::onShutdown()
	{
		// Remove the event area mini-map polygon again
		if (mPersonMissing)
		{
			if (nullptr != mIconEntity)
			{
				EventHelper(*this).removeEventIcon(*mIconEntity);
				mIconEntity = nullptr;
			}
		}

		mMissingPersonMessageProxy.unregister();

	}

	void ComplexFireEvent::onRun()
	{
		// Null pointer check in here. The factory did not check these pointers.

		// Get the event tag array to burn
		for (qsf::Entity* entity : mInflameEntities)
		{
			if (!inflameEntity(*entity))
			{
				QSF_ERROR("Entity of complex fire event could not be inflamed", QSF_REACT_NONE);
				abortEvent();
				return;
			}
		}
		qsf::Entity* metaEntity = getMap().getEntityById(mMetaEntityId);
		if (nullptr == metaEntity)
		{
			QSF_ERROR("There is no meta entity with this ID: " << mMetaEntityId, QSF_REACT_NONE);
			abortEvent();
			return;
		}

		// Set specific event name and supervisor messages
		if (mPersonDamage)
		{
			for (qsf::Entity* entity : mSpawnPointEntities)
			{
				qsf::Entity* personEntity = spawnEntity(*entity);
				if (nullptr == personEntity)
				{
					QSF_ERROR("Entity of complex fire event could not be spawned", QSF_REACT_NONE);
					abortEvent();
					return;
				}

				if (mPersonMissing)
				{
					EventHelper(*this).makeMissingPerson(*personEntity, qsf::StringHash(HintHelper::getRandomStringOfIdString(mInjuries)), false);
				}
				else
				{
					EventHelper(*this).makeInjuredPerson(*personEntity, HintHelper::getRandomStringOfIdString(mInjuries));
				}
			}
		}

		if (mPersonMissing)
		{
			qsf::Entity* metaEntity = getMap().getEntityById(mMetaEntityId);
			if (nullptr != metaEntity)
			{
				// Use the dog search radius as reference for the event radius
				const float searchRange = ActionRangeSpecsGroup::getInstanceSafe().getRescueDogSearchDistance() * 0.5f;

				mIconEntity = EventHelper(*this).createEventIconCircleArea(*metaEntity, searchRange, 0.0f);
			}
		}

		// Show supervisor message
		showSupervisorMessage(HintHelper::getRandomStringOfIdString(HintHelper(*this).getHintParameters().mSupervisorMessageIds));

		// Set all observers and objectives for the first target
		startObjectives(*metaEntity);

		// Set start hint observers after the event entity was added
		startHintObservers(*metaEntity);

		createEventLocationEntity(*metaEntity);
	}

	void ComplexFireEvent::updateFreeplayEvent(const qsf::Time& timePassed)
	{
		if (!mAllPersonsFound)
		{
			Objective* objective = ObjectiveHelper(*this).getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_FINDPERSON);
			if (nullptr != objective && objective->checkAccomplished())
			{
				onAllPersonsFound();
			}
		}

		// Check objectives for success or failure
		checkObjectivesState();
	}

	bool ComplexFireEvent::addEntityToEvent(qsf::Entity& targetEntity, eventspreadreason::Reason eventSpreadReason, bool newReason)
	{
		if (eventspreadreason::INJURY == eventSpreadReason)
		{
			// Get or create fail conditions
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);

			// Create and connect observers
			PersonDiedObserver& personDiedObserver = ObserverHelper(*this).createObserver<PersonDiedObserver>(targetEntity.getId());
			personDiedObserver.connectToObjective(failConditionPersonDiedObjective);

			// Call base implementation
			return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
		}
		else if (eventspreadreason::FIRE == eventSpreadReason)
		{
			// The entity could be a person so its injured not burned
			if (EntityHelper(targetEntity).isCivilPerson())
			{
				addEntityToEvent(targetEntity, eventspreadreason::INJURY, newReason);
			}

			// TODO(mk) Whats happen with non civil -> squad or gangster units? There are not invincible
		}

		// Call base implementation
		return FreeplayEvent::addEntityToEvent(targetEntity, eventSpreadReason, newReason);
	}

	void ComplexFireEvent::hintCallback(Observer& hintObserver)
	{
		showHint(hintObserver.getName(), false);
	}

	const qsf::Entity* ComplexFireEvent::getFocusEntity()
	{
		const qsf::Map& map = getMap();

		{ // Is the first building still burning?
			qsf::Entity* candidateEntity = map.getEntityById(mMetaEntityId);
			if (nullptr != candidateEntity && EntityHelper(*candidateEntity).isBurning())
			{
				return candidateEntity;
			}
		}

		const ObjectiveHelper objectiveHelper(*this);

		{ // Is there any other fire left?
			const Objective* extingushFireObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_EXTINGUISHFIRES);
			if (nullptr != extingushFireObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(extingushFireObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		{ // Are there any persons injured?
			const Objective* treatPersonsObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_TREATPERSONS);
			if (nullptr != treatPersonsObjective)
			{
				const qsf::Entity* candidateEntity = map.getEntityById(treatPersonsObjective->getRandomNeededEntityId());
				if (nullptr != candidateEntity)
				{
					return candidateEntity;
				}
			}
		}

		return getEventLocationEntity();
	}

	bool ComplexFireEvent::checkEventConfiguration()
	{
		// TODO(mk) Add Check if event has right configuration an can start now

		return true;
	}

	void ComplexFireEvent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base implementation
		FreeplayEvent::serialize(serializer);

		serializer.serialize(mMetaEntityId);
		serializer.serialize(mDeadPersonsToFail);
		serializer.serialize(mPersonDamage);
		serializer.serialize<std::string>(mInjuries);

		// TODO(mk) serialize mInflameEntities && mSpawnPointEntities
	}


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	void ComplexFireEvent::startObjectives(const qsf::Entity& targetEntity)
	{
		if (qsf::isInitialized(mDeadPersonsToFail) && mDeadPersonsToFail > 0)
		{
			// Create the fail condition
			Objective& failConditionPersonDiedObjective = ObjectiveHelper(*this).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_FAIL_DEADPERSONS);
			failConditionPersonDiedObjective.setNeededNumber(mDeadPersonsToFail);
		}
	}

	void ComplexFireEvent::startHintObservers(const qsf::Entity& targetEntity)
	{
		const HintHelper hintHelper(*this);
		hintHelper.createAndInitializeGeneralHintsAtInjured();
		hintHelper.createAndInitializeGeneralHintAtDamagedVehicle();

		Player* localPlayer = PlayerManager::getLocalPlayer();
		if (nullptr != localPlayer)
		{
			// Check if fireplane is available
			if (localPlayer->getUnitPool().getNumberMaxAvailableByName(CommandableComponent::UNITTAG_FIREFIGHTERS_FIREPLANE) <= 0)
			{
				// Hint no fireplane unit after X seconds
				createGeneralObserver<HintMessageGeneralObserver>(HintHelper::getRandomStringOfIdString(USE_FIREFIGHTINGPLANE_HINT_01 + "," + USE_FIREFIGHTINGPLANE_HINT_02)).initialize(
					qsf::MessageConfiguration(Messages::EM5_ORDER_UNIT, CommandableComponent::UNITTAG_FIREFIGHTERS_FIREPLANE), USE_FIREFIGHTINGPLANE_HINT_WAITTIME, false);
			}
		}
	}

	bool ComplexFireEvent::inflameEntity(qsf::Entity& entity)
	{
		const EntityHelper inflameEntityHelper(entity);

		// If persons should not be damaged do nothing
		if (!mPersonDamage && inflameEntityHelper.isCivilPerson())
			return true;

		// TODO(mk) What's happen with non civil -> squad or gangster units? There are not invincible

		if (!inflameEntityHelper.checkForInitializedEventIdComponent())
		{
			entity.getComponentSafe<qsf::RendererComponent>().setActive(true);

			// Set the entity on fire
			inflameEntityHelper.startFire(this);
			return true;
		}
		return false;
	}

	qsf::Entity* ComplexFireEvent::spawnEntity(qsf::Entity& spawnpoint)
	{
		// Check if entity has a spawn component
		PersonSpawnPointComponent* spawnPointEntity = spawnpoint.getComponent<PersonSpawnPointComponent>();
		if (nullptr == spawnPointEntity)
			return nullptr;

		return spawnPointEntity->spawnRandom();
	}

	void ComplexFireEvent::onAllPersonsFound()
	{
		mAllPersonsFound = true;
		if (nullptr != mIconEntity)
		{
			EventHelper(*this).removeEventIcon(*mIconEntity);
			mIconEntity = nullptr;
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
