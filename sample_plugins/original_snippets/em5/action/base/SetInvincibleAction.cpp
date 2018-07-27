// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/SetInvincibleAction.h"
#include "em5/map/EntityHelper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier SetInvincibleAction::ACTION_ID = "em5::SetInvincibleAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	SetInvincibleAction::SetInvincibleAction() :
		Action(ACTION_ID),
		mInvincible(true)
	{
		// Nothing here
	}

	SetInvincibleAction::~SetInvincibleAction()
	{
		// Nothing here
	}

	void SetInvincibleAction::init(bool invincible)
	{
		mInvincible = invincible;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void SetInvincibleAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mInvincible);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result SetInvincibleAction::updateAction(const qsf::Clock&)
	{
		// Use the entity helper for our problems
		EntityHelper(getEntity()).setInvincible(mInvincible);

		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
