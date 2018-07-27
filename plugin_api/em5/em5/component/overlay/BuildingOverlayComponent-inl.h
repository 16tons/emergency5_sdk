// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline BuildingOverlayComponent::~BuildingOverlayComponent()
	{
		// Nothing to do in here
	}

	inline const std::vector<BuildingOverlayProvider>& BuildingOverlayComponent::getRegisteredProviders() const
	{
		return mRegisteredProviders;
	}

	inline bool BuildingOverlayComponent::isEmpty() const
	{
		return isContentEmpty();
	}

	inline uint32 BuildingOverlayComponent::getNumberOfRequiredProviders() const
	{
		return mNumberOfRequiredProviders;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
