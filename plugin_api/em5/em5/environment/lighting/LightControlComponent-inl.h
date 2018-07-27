// Copyright (C) 2012-2014 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LightControlComponent::LightControlComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mEnableVehicleLights(false)
	{
		// Nothing to do
	}

	inline LightControlComponent::~LightControlComponent()
	{
		// Nothing to do in here
	}

	inline bool LightControlComponent::getEnableVehicleLights() const
	{
		return mEnableVehicleLights;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
