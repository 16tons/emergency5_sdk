// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/spawnpoint/civilist/VehicleSpawnPointComponent.h"

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
	const uint32 VehicleSpawnPointComponent::COMPONENT_ID = qsf::StringHash("em5::VehicleSpawnPointComponent");


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void VehicleSpawnPointComponent::serialize(qsf::BinarySerializer& serializer)
	{
		SpawnPointComponent::serialize(serializer);

		serializer.serializeAs<uint16>(mSpawnPointType);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
