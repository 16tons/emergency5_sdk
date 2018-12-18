// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/HistoricWellComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/game/Game.h"
#include "em5/game/animation/AnimationHelper.h"
#include "em5/EM5Helper.h"

#include <qsf/base/StringHash.h>
#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/renderer/animation/MeshAnimationComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HistoricWellComponent::COMPONENT_ID = qsf::StringHash("em5::HistoricWellComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HistoricWellComponent::HistoricWellComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mPumpEntityId(qsf::getUninitialized<uint64>()),
		mInternalActive(false)
	{
		// Nothing here
	}

	HistoricWellComponent::~HistoricWellComponent()
	{
		// Nothing here
	}

	void HistoricWellComponent::startPumpAnimation()
	{
		qsf::Entity* pumpEntity = getEntity().getMap().getEntityById(mPumpEntityId);
		if (nullptr != pumpEntity)
		{
			AnimationHelper animationHelper(*pumpEntity);
			animationHelper.playAnimation(animationHelper.getAnimationWellPumping(), true, false, qsf::Time::fromSeconds(0.3f));
		}
	}

	void HistoricWellComponent::stopPumpAnimation()
	{
		qsf::Entity* pumpEntity = getEntity().getMap().getEntityById(mPumpEntityId);
		if (nullptr != pumpEntity)
		{
			qsf::MeshAnimationComponent* meshAnimationComponent = pumpEntity->getComponent<qsf::MeshAnimationComponent>();
			if (nullptr != meshAnimationComponent)
			{
				meshAnimationComponent->stopAllAnimations();
			}
		}
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void HistoricWellComponent::onSetSimulating(bool simulating)
	{
		Game* game = Game::getInstance();
		if (nullptr == game || !game->isRunning())
			return;

		internalSetActive(simulating && isActive());
	}

	void HistoricWellComponent::onSetActive(bool active)
	{
		internalSetActive(isSimulating() && active);
	}

	void HistoricWellComponent::internalSetActive(bool active)
	{
		if (mInternalActive == active)
			return;

		mInternalActive = active;

		if (mInternalActive)
		{
			// Show overlay icon and connected minimap icon
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_WELL, true);

		}
		else
		{
			// Hide the overlay icon
			StatusOverlayComponent::hideIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_WELL);
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
