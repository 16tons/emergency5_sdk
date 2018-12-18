// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/StakeComponent.h"
#include "em5/action/base/PlayAnimationAction.h"
#include "em5/action/ActionPriority.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/map/EntityHelper.h"
#include "em5/EM5Helper.h"

#include <qsf_game/component/base/HiddenComponent.h>

#include <qsf/base/StringHash.h>
#include <qsf/math/Transform.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/logic/action/ActionComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 StakeComponent::COMPONENT_ID = qsf::StringHash("em5::StakeComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	StakeComponent::StakeComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mRescueTime(qsf::Time::fromSeconds(10.0f)),
		mWitchEntityId(qsf::getUninitialized<uint64>()),
		mActionPositionEntityId(qsf::getUninitialized<uint64>()),
		mCanBeIgnited(true),
		mInternalActive(false)
	{
		// Nothing here
	}

	StakeComponent::~StakeComponent()
	{
		// Nothing here
	}

	qsf::Transform StakeComponent::getActionTransform()
	{
		// The child entity of the pyre Entity has an tagged child what we use for positioning the guardsman
		qsf::Entity* actionEntity = getEntity().getMap().getEntityById(mActionPositionEntityId);
		if (nullptr != actionEntity)
		{
			return EntityHelper(*actionEntity).getTransform();
		}

		// Error, wrong editing
		return EntityHelper(getEntity()).getTransform();
	}

	void StakeComponent::turnOffIcons()
	{
		// Hide the overlay icon
		StatusOverlayComponent::hideIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_STAKE);
	}

	void StakeComponent::setWitchHidden(bool hidden)
	{
		qsf::Entity* witchEntity = getEntity().getMap().getEntityById(mWitchEntityId);
		if (nullptr != witchEntity)
		{
			qsf::game::HiddenComponent::setHidden(*witchEntity, hidden);
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void StakeComponent::onSetSimulating(bool simulating)
	{
		Game* game = Game::getInstance();
		if (nullptr == game || !game->isRunning())
			return;

		internalSetActive(simulating && isActive());
	}

	void StakeComponent::onSetActive(bool active)
	{
		internalSetActive(isSimulating() && active);
	}

	void StakeComponent::internalSetActive(bool active)
	{
		if (mInternalActive == active)
			return;

		mInternalActive = active;

		if (mInternalActive)
		{
			// Show overlay icon and connected minimap icon
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_STAKE, true);

			qsf::Entity* witchEntity = getEntity().getMap().getEntityById(mWitchEntityId);
			if (nullptr != witchEntity)
			{
				FireReceiverComponent* fireReceiverComponent = witchEntity->getComponent<FireReceiverComponent>();
				if (fireReceiverComponent)
				{
					//Make invulnerable against fire
					fireReceiverComponent->setActive(false);
				}

				// Set animation at startup. Using an action here avoid a resetting by the person animation controller, how will force idle animations
				witchEntity->getOrCreateComponentSafe<qsf::ActionComponent>().pushAction<PlayAnimationAction>(action::BLOCKING).init(AnimationHelper(*witchEntity).getAnimationWitchCaptured(), true, true);
			}
		}
		else
		{
			turnOffIcons();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
