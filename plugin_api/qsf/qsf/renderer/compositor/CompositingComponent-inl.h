// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CompositingComponent::CompositingComponent(Prototype* prototype) :
		Component(prototype)
	{
		// Nothing to do in here
	}

	inline CompositingComponent::~CompositingComponent()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::CompositingComponent methods      ]
	//[-------------------------------------------------------]
	inline void CompositingComponent::onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
