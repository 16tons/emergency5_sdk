// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CameraComponent::CameraComponent(Prototype* prototype) :
		RendererComponent(prototype),
		mProjectionType(PERSPECTIVE),
		mFov(45.0f),
		mNearClipDistance(0.1f),
		mFarClipDistance(1000.0f),
		mUseRenderingDistance(true),
		mLodBias(1.0f),
		// Primarily relevant for depth of field (DOF)
		mFocalNearPlaneDepth(0.1f),
		mFocalPlaneDepth(0.5f),
		mFocalFarPlaneDepth(100.0f),
		mFocalBlurrinessCutoff(0.0f),
		// Internal only
		mOgreCamera(nullptr),
		mOgreSceneNode(nullptr)
	{
		// Nothing to do in here
	}

	inline CameraComponent::ProjectionType CameraComponent::getProjectionType() const
	{
		return mProjectionType;
	}

	inline float CameraComponent::getFov() const
	{
		return mFov;
	}

	inline float CameraComponent::getNearClipDistance() const
	{
		return mNearClipDistance;
	}

	inline float CameraComponent::getFarClipDistance() const
	{
		return mFarClipDistance;
	}

	inline bool CameraComponent::getUseRenderingDistance() const
	{
		return mUseRenderingDistance;
	}

	inline float CameraComponent::getLodBias() const
	{
		return mLodBias;
	}

	inline float CameraComponent::getFocalNearPlaneDepth() const
	{
		return mFocalNearPlaneDepth;
	}

	inline float CameraComponent::getFocalPlaneDepth() const
	{
		return mFocalPlaneDepth;
	}

	inline float CameraComponent::getFocalFarPlaneDepth() const
	{
		return mFocalFarPlaneDepth;
	}

	inline float CameraComponent::getFocalBlurrinessCutoff() const
	{
		return mFocalBlurrinessCutoff;
	}

	inline const std::vector<RenderWindow*>& CameraComponent::getRenderWindows() const
	{
		return mRenderWindows;
	}

	inline Ogre::Camera* CameraComponent::getOgreCamera() const
	{
		return mOgreCamera;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	inline Ogre::SceneNode* CameraComponent::getOgreSceneNode() const
	{
		return mOgreSceneNode;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
