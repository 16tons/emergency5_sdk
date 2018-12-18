// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GhostComponent::GhostComponent(qsf::Prototype* prototype) :
		Component(prototype),
		mApplyDelay(2),
		mPlayerId(qsf::getUninitialized<uint32>()),
		mLocalPlayerId(qsf::getUninitialized<uint32>()),
		mAlphaValue(0.35f),
		mInternalIsGhost(false),
		mRendererIsGhost(false)
	{
		// Nothing to do in here
	}

	inline uint32 GhostComponent::getPlayerId() const
	{
		return mPlayerId;
	}

	inline bool GhostComponent::isGhost() const
	{
		return mInternalIsGhost;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
