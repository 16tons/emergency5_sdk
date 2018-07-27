// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/person/SpeakToPersonComponent.h"
#include "em5/component/movement/MoveToDespawnComponent.h"
#include "em5/component/overlay/StatusOverlayComponent.h"
#include "em5/game/Game.h"
#include "em5/game/minimap/MinimapHelper.h"
#include "em5/EM5Helper.h"

#include <qsf/base/StringHash.h>
#include <qsf/map/Entity.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 SpeakToPersonComponent::COMPONENT_ID = qsf::StringHash("em5::SpeakToPersonComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SpeakToPersonComponent::SpeakToPersonComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype)
	{
		// Nothing to do in here
	}

	SpeakToPersonComponent::~SpeakToPersonComponent()
	{
		// Nothing to do in here
	}

	void SpeakToPersonComponent::updateIcons()
	{
		if (isSimulating())
		{
			// Register mini map icon of the fleeing person
			MinimapHelper::registerIcon(getEntity(), MiniMapIconType::SEARCH);

			// Show overlay icon
			StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_SEARCH, true);
		}
		else
		{
			// Remove the entity from the mini map icon
			MinimapHelper::unregisterIcon(getEntity());

			// Hide the overlay icon
			StatusOverlayComponent::hideIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_SEARCH);
		}
	}

	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	void SpeakToPersonComponent::onSetSimulating(bool simulating)
	{
		if (simulating)
		{
			// Persons don't need a "move to despawn" component
			getEntity().destroyComponent<MoveToDespawnComponent>();

			if (EM5_GAME.isRunning())
			{
				// Register mini map icon of the fleeing person
				MinimapHelper::registerIcon(getEntity(), MiniMapIconType::SEARCH);

				// Show overlay icon
				StatusOverlayComponent::showIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_SEARCH, true);
			}
		}
		else
		{
			if (EM5_GAME.isRunning())
			{
				// Remove the entity from the mini map icon
				MinimapHelper::unregisterIcon(getEntity());

				// Hide the overlay icon
				StatusOverlayComponent::hideIcon(getEntity(), StatusOverlayComponent::STATUS_ICON_SEARCH);
			}
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
