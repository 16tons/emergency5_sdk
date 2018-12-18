// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PlacementComponent::~PlacementComponent()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Public qsf::PlacementComponent methods                ]
	//[-------------------------------------------------------]
	inline bool PlacementComponent::applyPlacement(glm::vec3&, glm::quat&)
	{
		// Default implementation does nothing at all
		return false;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline PlacementComponent::PlacementComponent(Prototype* prototype) :
		Component(prototype)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
