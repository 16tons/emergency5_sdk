// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BoostSignalComponent::BoostSignalComponent(Prototype* prototype) :
		Component(prototype)
	{
		// Nothing to do in here
	}

	inline BoostSignalComponent::~BoostSignalComponent()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	inline void BoostSignalComponent::onComponentPropertyChange(const Component& component, uint32 propertyId)
	{
		// Emit the Boost signal telling the world about the component property value change
		ComponentPropertyChange(component, propertyId);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
