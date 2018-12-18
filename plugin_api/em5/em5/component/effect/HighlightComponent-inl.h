// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool HighlightComponent::getHighlightFlags(HighlightComponent::HighlightFlags flag) const
	{
		return mHighlightFlags.isSet(flag);
	}

	inline HighlightComponent::HighlightFlags HighlightComponent::getCurrentHighlightType() const
	{
		return mCurrentHighlightType;
	}

	inline float HighlightComponent::getGroundSpotLightRadius() const
	{
		return mGroundSpotLightRadius;
	}

	inline float HighlightComponent::getSpotLightNearClipDistance() const
	{
		return mSpotLightNearClipDistance;
	}

	inline float HighlightComponent::getGroundHeight() const
	{
		return mGroundHeight;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
