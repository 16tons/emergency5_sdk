// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/base/AiLevelOfDetailComponent.h"

#include <qsf/base/StringHash.h>
#include <qsf/serialization/binary/BasicTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 AiLevelOfDetailComponent::COMPONENT_ID				= qsf::StringHash("em5::AiLevelOfDetailComponent");
	const uint32 AiLevelOfDetailComponent::ACTIVATION_PROPERTY_ID	= qsf::StringHash("AiLevelOfDetailComponent::ACTIVATION_PROPERTY_ID");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void AiLevelOfDetailComponent::update(const qsf::JobArguments&) // Ignoring the job arguments for now
	{
		// TODO(vs) Calculate new value instead of hardcoded true currently
		const bool newValue = true;
		if (newValue != mUseFullDetail)
		{
			mUseFullDetail = newValue;
			promotePropertyChange(ACTIVATION_PROPERTY_ID);
		}
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void AiLevelOfDetailComponent::serialize(qsf::BinarySerializer& serializer)
	{
		serializer.serialize(mUseFullDetail);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
