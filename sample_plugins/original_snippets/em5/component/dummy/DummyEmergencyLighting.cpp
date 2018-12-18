// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/dummy/DummyEmergencyLighting.h"

#include <qsf/base/StringHash.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 DummyEmergencyLighting::COMPONENT_ID				 = qsf::StringHash("em5::DummyEmergencyLighting");
	const uint32 DummyEmergencyLighting::EMERCENGY_LIGHT_PROPERTY_ID = qsf::StringHash("DummyEmergencyLighting::EMERCENGY_LIGHT_PROPERTY_ID");	// TODO(co) Violation of property naming convention


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
