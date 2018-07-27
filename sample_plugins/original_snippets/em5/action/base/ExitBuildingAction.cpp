// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/ExitBuildingAction.h"
#include "em5/component/building/BuildingComponent.h"
#include "em5/component/person/PersonComponent.h"
#include "em5/health/HealthComponent.h"
#include "em5/map/EntityHelper.h"

#include <qsf_ai/navigation/em4Router/RouterComponent.h>

#include <qsf/component/move/MovableComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier ExitBuildingAction::ACTION_ID = "em5::ExitBuildingAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	ExitBuildingAction::ExitBuildingAction() :
		Action(ACTION_ID),
		mCurrentState(STATE_INIT),
		mFade(false)
	{
		// Nothing here
	}

	ExitBuildingAction::~ExitBuildingAction()
	{
		// Nothing here
	}

	void ExitBuildingAction::init(bool fade)
	{
		mFade = fade;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void ExitBuildingAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serializeAs<uint16>(mCurrentState);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	bool ExitBuildingAction::onStartup()
	{
		// Get the most important objects first
		const qsf::MovableComponent* movableComponent = getEntity().getComponent<qsf::MovableComponent>();
		if (nullptr == movableComponent)
			return false;

		mBuildingEntity = movableComponent->getContainerEntity();
		if (!mBuildingEntity.valid())
			return false;

		// Test for valid building
		BuildingComponent* buildingComponent = mBuildingEntity->getComponent<BuildingComponent>();
		if (nullptr == buildingComponent)
			return false;

		return true;
	}

	qsf::action::Result ExitBuildingAction::updateAction(const qsf::Clock&)
	{
		if (!mBuildingEntity.valid())
			return qsf::action::RESULT_DONE;

		// Handle the current state
		switch (mCurrentState)
		{
			case STATE_INIT:
			{
				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_EXIT;
			}

			case STATE_EXIT:
			{
				// Exit the building now
				EntityHelper entityHelper(getEntity());
				entityHelper.leaveContainer();

				if (entityHelper.isPersonInjured())
				{
					const Injury* injury = getEntity().getComponentSafe<HealthComponent>().getInjury();
					if (nullptr != injury)
					{
						// Update bounding box manually to have placement check with the right collision, the currently value "BOX_LYING" is set without router component
						getEntity().getOrCreateComponent<qsf::ai::RouterComponent>();	// Used for the right placement in next step
						PersonComponent* personComponent = getEntity().getComponent<PersonComponent>();
						personComponent->setBoundingBoxType(PersonComponent::BOX_STANDING);	// Need this here to force an update with type BOX_LYING

						// Force bounding box update for injury type
						PersonComponent::BoundingBoxType boundingBoxType = (injury->isAffectingHealth() ? PersonComponent::BOX_STANDING : (injury->getIsKneeing() ? PersonComponent::BOX_KNEEING : PersonComponent::BOX_LYING));
						personComponent->setBoundingBoxType(boundingBoxType);
					}
				}

				// Immediately go to the next state; no break by intent
				mCurrentState = STATE_DONE;
				return qsf::action::RESULT_CONTINUE;
			}

			case STATE_DONE:
			{
				EntityHelper entityHelper(getEntity());
				entityHelper.placeEntityAtBuildingDoor(mBuildingEntity.getSafe(), false);

				// Update minimap icons
				BuildingComponent* buildingComponent = mBuildingEntity->getComponent<BuildingComponent>();
				if (nullptr != buildingComponent)
				{
					buildingComponent->updateIcons();
				}

				// Do fade in if requested
				if (mFade)
				{
					entityHelper.fadeIn();
				}

				// Done
				return qsf::action::RESULT_DONE;
			}
		}

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
