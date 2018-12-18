// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/logic/local/police/GangsterInBuildingLogic.h"
#include "em5/action/ActionPriority.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/base/ContainerCategory.h"
#include "em5/command/component/CommandableComponent.h"
#include "em5/freeplay/eventspread/EventSpreadReason.h"
#include "em5/freeplay/eventspread/EventSpreadHelper.h"
#include "em5/freeplay/objective/ObjectiveHelper.h"
#include "em5/logic/ObserverHelper.h"
#include "em5/logic/observer/TransportedToHqObserver.h"
#include "em5/map/EntityHelper.h"
#include "em5/specs/LogicSpecsGroup.h"
#include "em5/plugin/Messages.h"

#include <qsf/logic/action/ActionComponent.h>
#include <qsf/logic/action/WaitAction.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>
#include <qsf/message/MessageSystem.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 GangsterInBuildingLogic::GAMELOGIC_TYPE_ID = qsf::StringHash("em5::GangsterInBuildingLogic");


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	const uint32 GangsterInBuildingLogic::ACTION_PRIORITY = action::AUTOMATISM_STD;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	GangsterInBuildingLogic::GangsterInBuildingLogic() :
		qsf::GameLogic(GAMELOGIC_TYPE_ID),
		mAllowNormalPolice(true),
		mCanSpeakWithHostageTaker(false),
		mFakeObject(false),
		mInUseFromSpeaker(false),
		mCanSetHostageObjectives(false)
	{
		mGangsterTakeGiveUp = qsf::Random::getRandomChance(LogicSpecsGroup::getInstanceSafe().getPoliceHostageTakerGiveUpChance());
	}

	bool GangsterInBuildingLogic::isPoliceTeamComplete() const
	{
		std::vector<qsf::Entity*> policeUnits = mBuildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_PROGRESS);
		int numberValidPoliceUnits = 0;
		for (qsf::Entity* entity : policeUnits)
		{
			CommandableComponent* commandableComponent = entity->getComponent<CommandableComponent>();
			if (commandableComponent->isPoliceSEKPerson() || (mAllowNormalPolice && commandableComponent->isPolicePerson()))
			{
				++numberValidPoliceUnits;
			}
		}

		return (numberValidPoliceUnits >= 3);
	}

	void GangsterInBuildingLogic::onPoliceSpokeToHostageTaker()
	{
		// Freeplay event may be a null pointer in rare cases
		qsf::Entity& buildingEntity = mBuildingComponent->getEntity();
		FreeplayEvent* freeplayEvent = EntityHelper(buildingEntity).getFreeplayEvent();
		if (nullptr != freeplayEvent)
		{
			ObjectiveHelper objectiveHelper(*freeplayEvent);

			if (mGangsterTakeGiveUp)
			{
				// Get all gangster in building
				std::vector<qsf::Entity*> gangstersInBuilding = mBuildingComponent->getAllLinksByContainerType(container::CONTAINERTYPE_GANGSTER);

				// Leave the building
				qsf::Time waitTime = qsf::Time::ZERO;
				for (qsf::Entity* gangster : gangstersInBuilding)
				{
					qsf::ActionComponent& actionComponent = gangster->getOrCreateComponentSafe<qsf::ActionComponent>();
					actionComponent.clearPlan();
					actionComponent.pushAction<qsf::WaitAction>(action::BLOCKING).init(waitTime);
					actionComponent.pushAction<ExitBuildingAction>(action::BLOCKING).init(true);
					actionComponent.pushAction<qsf::WaitAction>(action::BLOCKING).init(qsf::Time::fromSeconds(15.0f));

					// The next one waits a little bit longer
					waitTime += qsf::Time::fromSeconds(1.0f);

					// TODO(mk) Here we use the same message as the attack building. Maybe we need a message extra for this
					// Send message
					qsf::MessageParameters parameters;
					parameters.setParameter("PolicemanId", getEntityId());
					parameters.setParameter("GangsterId", gangster->getId());
					parameters.setParameter("BuildingId", buildingEntity.getId());
					QSF_MESSAGE.emitMessage(qsf::MessageConfiguration(Messages::EM5_ACTION_ATTACK_BUILDING_FINISH, buildingEntity.getId()), parameters);

					{ // Set objective
						Objective& arrestGangsterObjective = ObjectiveHelper(*freeplayEvent).getOrCreateObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_ARRESTPERSONS);
						arrestGangsterObjective.increaseNeededNumber(gangster);

						TransportedToHqObserver& observerTransported = ObserverHelper(*freeplayEvent).createObserver<TransportedToHqObserver>(gangster->getId());
						observerTransported.connectToObjective(arrestGangsterObjective);
					}
				}

				// Force the event spread system to update the objectives for the hostages
				EventSpreadHelper::setEventIdComponent(freeplayEvent, buildingEntity, eventspreadreason::HOSTAGE);
			}
			else
			{
				// Unlock the next step
				mCanSetHostageObjectives = true;

				const uint32 objectiveTypeId = (mAllowNormalPolice ? ObjectiveHelper::OBJECTIVE_NEED_TAKEGANGSTEROUTSIDE_POLICE : ObjectiveHelper::OBJECTIVE_NEED_TAKEGANGSTEROUTSIDE_SEK);
				Objective& tookGangsterOutsideObjective = objectiveHelper.getOrCreateObjectiveByTypeId(objectiveTypeId);
				tookGangsterOutsideObjective.setNeededNumber(1);
			}

			// Negotiation is done
			Objective* speakToHostageTakerObjective = objectiveHelper.getObjectiveByTypeId(ObjectiveHelper::OBJECTIVE_NEED_SPEAKTO_HOSTAGETAKER);
			if (nullptr != speakToHostageTakerObjective)
			{
				speakToHostageTakerObjective->setAccomplished();
			}
		}
	}

	void GangsterInBuildingLogic::onPoliceAttackedBuilding()
	{
		// Freeplay event may be a null pointer in rare cases
		qsf::Entity& buildingEntity = mBuildingComponent->getEntity();
		FreeplayEvent* freeplayEvent = EntityHelper(buildingEntity).getFreeplayEvent();
		if (nullptr != freeplayEvent)
		{
			ObjectiveHelper objectiveHelper(*freeplayEvent);

			// This place is called for every policeman that attacks the building; change the objectives only once (that's what mCanSetHostageObjectives is for)
			const bool isLastPolicemanAttacking = (mCanSetHostageObjectives && !mBuildingComponent->isContainingGangsters());
			if (isLastPolicemanAttacking)
			{
				mCanSetHostageObjectives = false;

				// Force the eventspread system to update the objectives for the hostages
				EventSpreadHelper::setEventIdComponent(freeplayEvent, buildingEntity, eventspreadreason::HOSTAGE);
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GameLogic methods                 ]
	//[-------------------------------------------------------]
	bool GangsterInBuildingLogic::onStartup()
	{
		mBuildingComponent = getEntity()->getComponent<BuildingComponent>();

		return true;
	}

	void GangsterInBuildingLogic::onShutdown()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
