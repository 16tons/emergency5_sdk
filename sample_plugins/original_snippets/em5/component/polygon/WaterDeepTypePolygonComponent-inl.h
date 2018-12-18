// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline WaterDeepTypePolygonComponent::WaterDeepTypePolygonComponent(qsf::Prototype* prototype) :
		qsf::PolygonComponent(prototype),
		mWaterDeepType(SHALLOW_WATER)
	{
		// Nothing to do in here
	}

	inline WaterDeepTypePolygonComponent::~WaterDeepTypePolygonComponent()
	{
		// Nothing to do in here
	}

	inline WaterDeepTypePolygonComponent::WaterDeepType WaterDeepTypePolygonComponent::getWaterDeepType() const
	{
		return mWaterDeepType;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
