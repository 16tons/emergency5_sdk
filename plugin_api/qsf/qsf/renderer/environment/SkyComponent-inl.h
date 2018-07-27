// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SkyComponent::SkyComponent(Prototype* prototype) :
		Component(prototype),
		mOgreEntity(nullptr),
		mOgreSceneNode(nullptr)
	{
		// Nothing to do in here
	}

	inline SkyComponent::~SkyComponent()
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
