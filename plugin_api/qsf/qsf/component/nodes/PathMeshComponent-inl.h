// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PathMeshComponent::PathMeshComponent(Prototype* prototype) :
		PathComponent(prototype),
		mFlipXAxis(false),
		mFlipYAxis(false),
		mBlendOutLengthAtStart(0.0f),
		mBlendOutLengthAtEnd(0.0f),
		mCollisionLeftOffset(0.0f),
		mCollisionRightOffset(0.0f),
		mCollisionBottomOffset(0.0f),
		mCollisionTopOffset(0.0f),
		mDirtyMesh(false)	// By default, there are no nodes so no procedural path mesh is required
	{
		// Nothing to do in here
	}

	inline PathMeshComponent::~PathMeshComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& PathMeshComponent::getBaseMesh() const
	{
		return mBaseMesh;
	}

	inline bool PathMeshComponent::getFlipXAxis() const
	{
		return mFlipXAxis;
	}

	inline bool PathMeshComponent::getFlipYAxis() const
	{
		return mFlipYAxis;
	}

	inline float PathMeshComponent::getBlendOutLengthAtStart() const
	{
		return mBlendOutLengthAtStart;
	}

	inline float PathMeshComponent::getBlendOutLengthAtEnd() const
	{
		return mBlendOutLengthAtEnd;
	}

	inline float PathMeshComponent::getCollisionLeftOffset() const
	{
		return mCollisionLeftOffset;
	}

	inline float PathMeshComponent::getCollisionRightOffset() const
	{
		return mCollisionRightOffset;
	}

	inline float PathMeshComponent::getCollisionBottomOffset() const
	{
		return mCollisionBottomOffset;
	}

	inline float PathMeshComponent::getCollisionTopOffset() const
	{
		return mCollisionTopOffset;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
