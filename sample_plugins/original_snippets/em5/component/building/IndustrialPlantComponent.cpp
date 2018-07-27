// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/building/IndustrialPlantComponent.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	const uint32 IndustrialPlantComponent::COMPONENT_ID = qsf::StringHash("em5::IndustrialPlantComponent");


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	void IndustrialPlantComponent::startSimulation()
	{
		// Call base class implementation
		ContaminationComponent::startSimulation();
	}

	void IndustrialPlantComponent::stopSimulation()
	{
		// Call base class implementation
		ContaminationComponent::stopSimulation();
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	void IndustrialPlantComponent::serialize(qsf::BinarySerializer& serializer)
	{
		// Call base class implementation
		ContaminationComponent::serialize(serializer);
	}

	void IndustrialPlantComponent::onSetDebug(bool debug)
	{
		// Call base class implementation
		ContaminationComponent::onSetDebug(debug);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
