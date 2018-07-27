// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/equipment/DropEquipmentAction.h"
#include "em5/map/EntityHelper.h"

#include <qsf/component/base/TransformComponent.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DropEquipmentAction::ACTION_ID = "em5::DropEquipmentAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DropEquipmentAction::DropEquipmentAction() :
		Action(ACTION_ID)
	{
		// Nothing here
	}

	DropEquipmentAction::~DropEquipmentAction()
	{
		// Nothing here
	}

	void DropEquipmentAction::init(const qsf::Time& dropTime)
	{
		mDropTime = dropTime;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DropEquipmentAction::serialize(qsf::BinarySerializer& serializer)
	{
		// TODO(co) Anything to do in here?
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result DropEquipmentAction::updateAction(const qsf::Clock& clock)
	{
		// Countdown
		mDropTime -= clock.getTimePassed();

		if (mDropTime <= qsf::Time::ZERO)
		{
			EntityHelper(getEntity()).deleteActiveEquipment();
			return qsf::action::RESULT_DONE;
		}
		else
		{
			// Get the equipment entity
			const qsf::Entity* entity = EntityHelper(getEntity()).getActiveEquipmentEntity();
			if (nullptr != entity)
			{
				// Let the active entity fall
				qsf::TransformComponent* transformComponent = entity->getComponent<qsf::TransformComponent>();
				if (nullptr != transformComponent)
				{
					glm::vec3 position = transformComponent->getPosition();
					position.y -= clock.getTimePassed().getSeconds();
					transformComponent->setPosition(position);

					return qsf::action::RESULT_CONTINUE;
				}
			}
		}

		// Error, abort action
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
