// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/PoliceDogTargetComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/application/Application.h"
#include "em5/game/Game.h"
#include "em5/game/minimap/MinimapHelper.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PoliceDogTargetComponent::COMPONENT_ID = qsf::StringHash("em5::PoliceDogTargetComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	PoliceDogTargetComponent::PoliceDogTargetComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype)
	{
		// Nothing here
	}

	PoliceDogTargetComponent::~PoliceDogTargetComponent()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void PoliceDogTargetComponent::onSetSimulating(bool simulating)
	{
		Game* game = Game::getInstance();
		if (nullptr != game && game->isRunning())
		{
			if (simulating)
			{
				// Register mini map icon of the fleeing person
				MinimapHelper::registerIcon(getEntity(), MiniMapIconType::EVENT);

				// Show overlay icon
				StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS, true);
			}
			else
			{
				// Remove the entity from the mini map icon
				MinimapHelper::unregisterIcon(getEntity());

				// Hide the overlay icon
				StatusOverlayComponent::hideIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_EVENT_VARIOUS);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
