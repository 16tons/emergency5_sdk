// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline TraceDecalComponent::TraceDecalComponent(Prototype* prototype) :
		DecalComponent(prototype),
		mOgreEntity(nullptr),
		mDirtyMesh(false),	// By default, there are no nodes so no mesh is required
		mDestroyWhenOutOfNodes(false)
	{
		// Nothing to do in here
	}

	inline TraceDecalComponent::~TraceDecalComponent()
	{
		// Nothing to do here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
