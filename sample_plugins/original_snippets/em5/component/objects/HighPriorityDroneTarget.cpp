// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/objects/HighPriorityDroneTarget.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 HighPriorityDroneTarget::COMPONENT_ID = qsf::StringHash("em5::HighPriorityDroneTarget");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	HighPriorityDroneTarget::HighPriorityDroneTarget(qsf::Prototype* prototype) :
		Component(prototype)
	{
		// Nothing here
	}

	HighPriorityDroneTarget::~HighPriorityDroneTarget()
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
