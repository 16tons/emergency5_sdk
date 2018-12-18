// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ParticlesComponent::ParticlesComponent(Prototype* prototype) :
		RendererComponent(prototype),
		mTriedLoading(false),
		mScaleVelocity(1.0f),
		mKeepLocal(false),
		mTightBoundingBox(false),
		mEmitterEnabled(true),
		mEmitterIntensity(1.0f),
		mIterationInterval(0.05f),
		mFixedTimeout(0.0f),
		mParticleUniverseParticleSystemInstanceManager(nullptr),
		mParticleUniverseParticleSystem(nullptr),
		mParticleSystemListener(nullptr),
		mOriginalScaleVelocity(1.0f),
		mTransformChangeSinceLastUpdate(true),
		mOgreSceneNode(nullptr),
		mOgreMovableObject(nullptr),
		mDelayPassed(Time::ZERO),
		mDelayTime(Time::ZERO),
		mFirstVisible(false),
		mRealTimeParticles(false),
		mMaximumRenderingDistance(60.0f)
	{
		// By default, restart the particles animation on activation
		setRestartOnActivation(true);
	}

	inline ParticlesComponent::~ParticlesComponent()
	{
		// Nothing to do in here
	}

	inline const AssetProxy& ParticlesComponent::getParticles() const
	{
		return mParticlesAssetProxy;
	}

	inline float ParticlesComponent::getScaleVelocity() const
	{
		return mScaleVelocity;
	}

	inline bool ParticlesComponent::getKeepLocal() const
	{
		return mKeepLocal;
	}

	inline bool ParticlesComponent::getTightBoundingBox() const
	{
		return mTightBoundingBox;
	}

	inline bool ParticlesComponent::getEmitterEnabled() const
	{
		return mEmitterEnabled;
	}

	inline float ParticlesComponent::getEmitterIntensity() const
	{
		return mEmitterIntensity;
	}

	inline float ParticlesComponent::getIterationInterval() const
	{
		return mIterationInterval;
	}

	inline float ParticlesComponent::getFixedTimeout() const
	{
		return mFixedTimeout;
	}

	inline bool ParticlesComponent::getRestartOnActivation() const
	{
		return mInternalFlags.isSet(RESTART_ON_ACTIVATION);
	}

	inline void ParticlesComponent::setRestartOnActivation(bool restartOnActivation)
	{
		mInternalFlags.set(RESTART_ON_ACTIVATION, restartOnActivation);
	}

	inline bool ParticlesComponent::getRepeatAnimation() const
	{
		return mInternalFlags.isSet(REPEAT_ANIMATION);
	}

	inline void ParticlesComponent::setRepeatAnimation(bool repeatAnimation)
	{
		mInternalFlags.set(REPEAT_ANIMATION, repeatAnimation);
	}

	inline float ParticlesComponent::getDelayTime() const
	{
		return mDelayTime.getSeconds();
	}

	inline void ParticlesComponent::showAnimation()
	{
		mInternalFlags.set(SHOW_ANIMATION);
	}

	inline bool ParticlesComponent::isFinished() const
	{
		return mInternalFlags.isSet(PLAYBACK_FINISHED);
	}

	inline float ParticlesComponent::getFastForward() const
	{
		return mFastForward.getSeconds();
	}

	inline void ParticlesComponent::setFastForward(float fastForward)
	{
		mFastForward.setSeconds(fastForward);
	}

	inline Ogre::MovableObject* ParticlesComponent::getOgreMovableObject()
	{
		return reinterpret_cast<Ogre::MovableObject*>(mOgreMovableObject);
	}

	inline bool ParticlesComponent::getRealTimeParticles() const
	{
		return mRealTimeParticles;
	}

	inline float ParticlesComponent::getMaximumRenderingDistance() const
	{
		return mMaximumRenderingDistance;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	inline Ogre::SceneNode* ParticlesComponent::getOgreSceneNode() const
	{
		return mOgreSceneNode;
	}


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	inline bool ParticlesComponent::isGridManaged() const
	{
		return true;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
