// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/GetUninitialized.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline RendererComponent::RendererComponent(Prototype* prototype) :
		Component(prototype),
		mCastShadows(true),
		mRenderQueueId(getUninitialized<uint32>()),
		mRenderingDistanceFactor(1.0f),
		mDefaultRenderQueueId(getUninitialized<uint32>()),
		mFlipCulling(false)
	{
		// Nothing to do in here
	}

	inline RendererComponent::~RendererComponent()
	{
		// Nothing to do in here
	}

	inline bool RendererComponent::getCastShadows() const
	{
		return mCastShadows;
	}

	inline uint32 RendererComponent::getRenderQueueId() const
	{
		return mRenderQueueId;
	}

	inline float RendererComponent::getRenderingDistanceFactor() const
	{
		return mRenderingDistanceFactor;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	inline Ogre::SceneNode* RendererComponent::getOgreSceneNode() const
	{
		return nullptr;
	}

	inline Ogre::v1::Entity* RendererComponent::getOgreEntity() const
	{
		return nullptr;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	inline bool RendererComponent::isGridManaged() const
	{
		return false;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline bool RendererComponent::shouldBeVisible() const
	{
		return (isActive() && isRunning() && !isReallyHidden());
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
