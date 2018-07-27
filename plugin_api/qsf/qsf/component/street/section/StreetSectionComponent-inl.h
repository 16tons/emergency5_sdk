// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline StreetSectionComponent::StreetSectionComponent(Prototype* prototype) :
		StreetComponent(prototype),
		mFlipXAxis(false),
		mFlipYAxis(false),
		mBlendOutLengthAtStart(0.0f),
		mBlendOutLengthAtEnd(0.0f),
		mDirtyMesh(false)	// By default, there are no nodes so no procedural street mesh is required,
	{
		// A street section always has exactly two street gateways
		mStreetGateways.resize(2);
		mRegisteredStreetGateways.resize(2);

		// Usability: By default, show gateways in debug mode
		mDebugFlagSet.set(DEBUG_GATEWAYS);
	}

	inline StreetSectionComponent::~StreetSectionComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& StreetSectionComponent::getBaseMesh() const
	{
		return mBaseMesh;
	}

	inline bool StreetSectionComponent::getFlipXAxis() const
	{
		return mFlipXAxis;
	}

	inline bool StreetSectionComponent::getFlipYAxis() const
	{
		return mFlipYAxis;
	}

	inline float StreetSectionComponent::getBlendOutLengthAtStart() const
	{
		return mBlendOutLengthAtStart;
	}

	inline float StreetSectionComponent::getBlendOutLengthAtEnd() const
	{
		return mBlendOutLengthAtEnd;
	}

	inline const StreetGatewayLink& StreetSectionComponent::getBeginStreetGatewayLink() const
	{
		QSF_CHECK(!mStreetGateways.empty(), "Invalid street gateway index", QSF_REACT_NONE);
		return mStreetGateways[BEGIN_INDEX].getStreetGatewayLink();
	}

	inline const StreetGatewayLink& StreetSectionComponent::getEndStreetGatewayLink() const
	{
		QSF_CHECK(mStreetGateways.size() > 1, "Invalid street gateway index", QSF_REACT_NONE);
		return mStreetGateways[END_INDEX].getStreetGatewayLink();
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	inline bool StreetSectionComponent::ignoreOwnerRunningState() const
	{
		// Do always startup the component, regardless of the owner running state
		return true;
	}

	inline bool StreetSectionComponent::implementsOnComponentPropertyChange() const
	{
		// React on component property change
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
