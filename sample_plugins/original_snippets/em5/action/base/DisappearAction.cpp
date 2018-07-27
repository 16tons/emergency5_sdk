// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/action/base/DisappearAction.h"
#include "em5/component/effect/FadeEffectComponent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const qsf::NamedIdentifier DisappearAction::ACTION_ID = "em5::DisappearAction";


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	DisappearAction::DisappearAction() :
		Action(ACTION_ID),
		mDisappearTime(qsf::Time::fromSeconds(1.0f)),
		mDeleteOwner(true)
	{
		// Nothing here
	}

	DisappearAction::~DisappearAction()
	{
		// Nothing here
	}

	void DisappearAction::init(const qsf::Time& disappearTime, bool deleteOwner)
	{
		mDisappearTime = disappearTime;
		mDeleteOwner = deleteOwner;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Actions methods                   ]
	//[-------------------------------------------------------]
	void DisappearAction::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mDisappearTime);
		serializer.serialize(mDeleteOwner);
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Action methods                 ]
	//[-------------------------------------------------------]
	qsf::action::Result DisappearAction::updateAction(const qsf::Clock&)
	{
		// Start effect via component
		// When the owner shouldn't be deleted after fade out hide it
		getEntity().getOrCreateComponentSafe<FadeEffectComponent>().fadeOut(mDisappearTime, mDeleteOwner);

		// Done
		return qsf::action::RESULT_DONE;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
