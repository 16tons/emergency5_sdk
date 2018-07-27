// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline WheelComponent::WheelComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mRadius(1.0f),
		mAngleInRadians(0.0f),
		mWheelType(WHEELTYPE_UNDEFINED),
		mChainSpeedV(1.0f),
		mChainSpeedU(0.0f),
		mDistanceFromTurnPoint(1.0f),
		mChainOffsetU(0.0f),
		mChainOffsetV(0.0f)
	{
		// Nothing to do in here
	}

	inline WheelComponent::~WheelComponent()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
