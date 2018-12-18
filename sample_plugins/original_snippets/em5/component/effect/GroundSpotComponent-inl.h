// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GroundSpotComponent::~GroundSpotComponent()
	{
		// Nothing to do in here
	}

	inline float GroundSpotComponent::getRadius() const
	{
		return mGroundSpotLightRadius;
	}

	inline qsf::Entity* GroundSpotComponent::getGroundSpotLightEntity() const
	{
		return mGroundSpotLightEntity.get();
	}

	inline float GroundSpotComponent::getGroundSpotLightHeight() const
	{
		return mGroundSpotLightHeight;
	}

	inline void GroundSpotComponent::setGroundSpotLightHeight(float groundSpotLightHeight)
	{
		if (mGroundSpotLightHeight != groundSpotLightHeight)
		{
			mGroundSpotLightHeight = groundSpotLightHeight;

			// Force update
			setupGroundLight();
		}
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
