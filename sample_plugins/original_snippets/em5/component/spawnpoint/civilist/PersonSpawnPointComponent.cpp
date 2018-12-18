// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/spawnpoint/civilist/PersonSpawnPointComponent.h"

#include <qsf/base/StringHash.h>
#include <qsf/serialization/binary/StlTypeSerialization.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 PersonSpawnPointComponent::COMPONENT_ID = qsf::StringHash("em5::PersonSpawnPointComponent");


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void PersonSpawnPointComponent::serialize(qsf::BinarySerializer& serializer)
	{
		SpawnPointComponent::serialize(serializer);

		serializer.serializeAs<uint16>(mSpawnPointType);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
