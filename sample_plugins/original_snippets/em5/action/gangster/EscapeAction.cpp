// Copyright (C) 2012-2015 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/gangster/EscapeAction.h"

#include <qsf_game/component/event/EventTagManagerComponent.h>

#include <qsf/math/Random.h>
#include <qsf/QsfHelper.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void EscapeAction::catched()
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	EscapeAction::EscapeAction(uint32 actionId) :
		qsf::ai::MoveAction(actionId)
	{
		// Nothing here
	}

	EscapeAction::~EscapeAction()
	{
		// Nothing here
	}

	qsf::Entity* EscapeAction::getTargetEntity() const
	{
		if (!mEscapeTargetTag.empty())
		{
			const auto& list = qsf::game::EventTagManagerComponent::getEventTagComponentsByTag(qsf::StringHash(mEscapeTargetTag), QSF_MAINMAP);
			if (!list.empty())
			{
				return &qsf::Random::getAnyOf(list)->getEntity();
			}
		}
		return nullptr;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
