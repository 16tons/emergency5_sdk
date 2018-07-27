// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/TargetFoundCounterComponent.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 TargetFoundCounterComponent::COMPONENT_ID = qsf::StringHash("em5::TargetFoundCounterComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	TargetFoundCounterComponent::TargetFoundCounterComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mNumberFoundTargets(0)
	{
		// Nothing to do in here
	}

	TargetFoundCounterComponent::~TargetFoundCounterComponent()
	{
		// Nothing to do in here
	}

	int TargetFoundCounterComponent::getNumber() const
	{
		return mNumberFoundTargets;
	}

	void TargetFoundCounterComponent::increaseNumber()
	{
		++mNumberFoundTargets;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
