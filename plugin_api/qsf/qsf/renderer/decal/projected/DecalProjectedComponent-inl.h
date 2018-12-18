// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline DecalProjectedComponent::DecalProjectedComponent(Prototype* prototype) :
		DecalComponent(prototype),
		mOgreEntity(nullptr),
		mDirtyMesh(false)	// By default, there are no nodes so no procedural decal mesh is required
	{
		// Nothing to do in here
	}

	inline DecalProjectedComponent::~DecalProjectedComponent()
	{
		// Nothing to do here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
