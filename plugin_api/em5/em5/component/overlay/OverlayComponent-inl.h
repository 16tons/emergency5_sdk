// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Protected virtual em5::OverlayComponent methods       ]
	//[-------------------------------------------------------]
	bool OverlayComponent::isContentEmpty() const
	{
		// Nothing to do in here

		// No content
		return false;
	}

	void OverlayComponent::updateContent(Rocket::Core::ElementDocument&)
	{
		// Nothing to do in here
	}

	void OverlayComponent::updateAnimation(const qsf::JobArguments&)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
