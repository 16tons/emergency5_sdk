// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/renderer/component/RendererComponent.h"
#include "qsf/renderer/map/JobProxyMovableObjectRenderingListener.h"
#include "qsf/job/JobProxy.h"
#include "qsf/math/EnumBitmask.h"
#include "qsf/reflection/type/CampQsfAssetProxy.h"

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class MovableObject;
}
namespace ParticleUniverse
{
	class ParticleSystem;
}
namespace qsf
{
	class ParticleSystemListener;
	class ParticleUniverseParticleSystemInstanceManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Particles component class
	*
	*  @note
	*    - The external library "Particle Universe" (PU) ( http://www.fxpression.com/ ) is used
	*    - The internal Particle Universe particle system instance can't be exposed to outsiders because it's reused between multiple particles components
	*/
	class QSF_API_EXPORT ParticlesComponent : public RendererComponent, public JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class ParticleManager;		// Needs to manage the particles update
		friend class RendererDebugGroup;
		friend class ParticleSystemListener;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;						///< "qsf::ParticlesComponent" unique component ID
		static const uint32 PARTICLES;							///< "Particles" unique class property ID inside the class
		static const uint32 SCALE_VELOCITY;						///< "ScaleVelocity" unique class property ID inside the class
		static const uint32 KEEP_LOCAL;							///< "KeepLocal" unique class property ID inside the class
		static const uint32 TIGHT_BOUNDING_BOX;					///< "TightBoundingBox" unique class property ID inside the class
		static const uint32 EMITTER_ENABLED;					///< "EmitterEnabled" unique class property ID inside the class
		static const uint32 ITERATION_INTERVAL;					///< "IterationInterval" unique class property ID inside the class
		static const uint32 FIXED_TIMEOUT;						///< "FixedTimeout" unique class property ID inside the class
		static const uint32 VISIBLE_BY_CAMERA_NON_ZERO_START;	///< "VisibleByCameraNonZeroStart" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		inline explicit ParticlesComponent(Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ParticlesComponent();

		/**
		*  @brief
		*    Return the particles asset to use
		*
		*  @return
		*    The particles asset to use (e.g. "sample/particles/fire/campfire"), do no destroy the returned instance
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline const AssetProxy& getParticles() const;

		/**
		*  @brief
		*    Set the particles asset to use
		*
		*  @param[in] particles
		*    The particles asset to use (e.g. "sample/particles/fire/campfire")
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setParticles(const AssetProxy& particlesAssetProxy);

		/**
		*  @brief
		*    Return the velocity scale
		*
		*  @return
		*    The velocity scale, to scale the complete particle system by 50% set a velocity scale of 0.5 and a transform component scale of [0.5, 0.5, 0.5]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getScaleVelocity() const;

		/**
		*  @brief
		*    Set the velocity scale
		*
		*  @param[in] scaleVelocity
		*    The velocity scale, to scale the complete particle system by 50% set a velocity scale of 0.5 and a transform component scale of [0.5, 0.5, 0.5]
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setScaleVelocity(float scaleVelocity);

		/**
		*  @brief
		*    Return whether or not the particles are emitted relative to the system
		*
		*  @return
		*    If "true" the particles are emitted relative to the system, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getKeepLocal() const;

		/**
		*  @brief
		*    Set whether or not the particles are emitted relative to the system
		*
		*  @param[in] keepLocal
		*    If this property is set to "true", the particles are emitted relative to the system, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setKeepLocal(bool keepLocal);

		/**
		*  @brief
		*    Return whether or not the bounding box is wrapped tight around the particle system
		*
		*  @return
		*    Is "true" when the bounding box is wrapped tight around the particle system or "false" when the
		*    bounding box is only increasing and doesn't shrink when the particle system shrinks
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getTightBoundingBox() const;

		/**
		*  @brief
		*    Set whether or not the bounding box is wrapped tight around the particle system
		*
		*  @param[in] tightBoundingBox
		*    "true" when the bounding box is wrapped tight around the particle system or "false" when the
		*    bounding box is only increasing and doesn't shrink when the particle system shrinks
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setTightBoundingBox(bool tightBoundingBox);

		/**
		*  @brief
		*    Return whether or not the particle emitter is enabled
		*
		*  @return
		*    "true" if the particle emitter is enabled, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getEmitterEnabled() const;

		/**
		*  @brief
		*    Set whether or not the particle emitter is enabled
		*
		*  @param[in] emitterEnabled
		*    "true" if the particle emitter is enabled, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setEmitterEnabled(bool emitterEnabled);

		/**
		*  @brief
		*    Return the particle emitter intensity
		*
		*  @return
		*    Particle emitter intensity (0..1, none..full, negative interval in case particles quality is suppressed)
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent so artists don't mess around with the setting
		*/
		inline float getEmitterIntensity() const;

		/**
		*  @brief
		*    Set the particle emitter intensity
		*
		*  @param[in] emitterIntensity
		*    Particle emitter intensity (0..1, none..full). When providing negative values (internally converted to 0..1), particles quality reduction is suppressed. Somewhat hackish solution, but this way we don't need an additional property or flag.
		*  @param[in] enforceSet
		*    By default, internal work is only performed in case there's a real state change, by setting this parameter to "true" you enforce an update
		*
		*  @note
		*    - Not connected to the CAMP reflection system by intent so artists don't mess around with the setting
		*/
		void setEmitterIntensity(float emitterIntensity, bool enforceSet = false);

		/**
		*  @brief
		*    Return the value that determines in which intervals the system must be the updated
		*
		*  @return
		*    The value that determines in which intervals the system must be the updated
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getIterationInterval() const;

		/**
		*  @brief
		*    Set the value that determines in which intervals the system must be the updated
		*
		*  @param[in] iterationInterval
		*    The value that determines in which intervals the system must be the updated
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setIterationInterval(float iterationInterval);

		/**
		*  @brief
		*    Return the past time after which the particle system automatically stops
		*
		*  @return
		*    The past time after which the particle system automatically stops (in seconds)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getFixedTimeout() const;

		/**
		*  @brief
		*    Set the past time after which the particle system automatically stops
		*
		*  @param[in] fixedTimeout
		*    The past time after which the particle system automatically stops (in seconds)
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setFixedTimeout(float fixedTimeout);

		/**
		*  @brief
		*    Return whether or not to restart the particles on activation
		*
		*  @return
		*    "true" to restart the particles on activation, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getRestartOnActivation() const;

		/**
		*  @brief
		*    Set whether or not to restart the particles on activation
		*
		*  @param[in] restartOnActivation
		*    "true" to restart the particles on activation, else "false"
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setRestartOnActivation(bool restartOnActivation);

		/**
		*  @brief
		*    Checks if the animation is repating
		*
		*  @return
		*    "true" when the animation is repeat and "false" when not
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline bool getRepeatAnimation() const;

		/**
		*  @brief
		*    Set it to repeat the particle animation
		*
		*  @param[in] repeatAnimation
		*    "true" to activate the repeat and "false" to stop it
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline void setRepeatAnimation(bool repeatAnimation);

		/**
		*  @brief
		*    Return the delay time we want to wait when the particles are started
		*
		*  @return
		*    Delay time in seconds
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		inline float getDelayTime() const;

		/**
		*  @brief
		*    Set the delay time we want to wait when the particles are started
		*
		*  @param[in] delayTime
		*    Time in seconds
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setDelayTime(float delayTime);

		/**
		*  @brief
		*    Starts the animation of the particles
		*/
		inline void showAnimation();

		/**
		*  @brief
		*    Particles are destroyed
		*/
		inline bool isFinished() const;

		/**
		*  @brief
		*    Return how many seconds particles will be forwarded after they are visible again by the camera
		*/
		inline float getFastForward() const;

		/**
		*  @brief
		*    Set how many seconds particles will be forwarded after they are visible again by the camera
		*/
		inline void setFastForward(float fastForward);

		/**
		*  @brief
		*    Return if we have real time particles
		*/
		inline bool getRealTimeParticles() const;

		/**
		*  @brief
		*    Real time particles are not influenced by game speed
		*/
		void setRealTimeParticles(bool realTimeParticles);

		/**
		*  @brief
		*    Return the maximum rendering distance
		*/
		inline float getMaximumRenderingDistance() const;

		/**
		*  @brief
		*    Set the maximum rendering distance
		*/
		void setMaximumRenderingDistance(float maximumRenderingDistance);

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the dummy OGRE movable object
		*
		*  @return
		*    The dummy OGRE movable object instance, can be a null pointer, do no destroy the returned instance
		*
		*  @note
		*    - Don't access the dummy OGRE movable object instance if you don't have to
		*/
		inline Ogre::MovableObject* getOgreMovableObject();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::RendererComponent methods         ]
	//[-------------------------------------------------------]
	public:
		inline virtual Ogre::SceneNode* getOgreSceneNode() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::RendererComponent methods      ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateOgreObjectRenderingDistance() const override;
		virtual void updateOgreObjectCastShadows() const override;
		virtual void updateOgreObjectRenderQueue(uint32 renderQueueId) override;
		inline virtual bool isGridManaged() const override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;


		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingStartup() override;
		virtual void onObjectRenderingShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void createParticleSystem();
		void updateOgreObjectRenderingDistance(Ogre::SceneNode& ogreSceneNode) const;

		/**
		*  @brief
		*    Destroy the Particle Universe particle system
		*
		*  @param[in] destroyDummy
		*    Destroy the dummy which is used for creating the visual particle system in case it gets visible as well?
		*/
		void destroyParticleSystem(bool destroyDummy);

		/**
		*  @brief
		*    Set the Particle Universe template to use, internal part
		*
		*  @param[in] ignoreActiveState
		*    Ignore active state? Meaning the internal Particle Universe particles system will also be created if this particles component is not active.
		*/
		void setTemplateInternal(bool ignoreActiveState = false);

		/**
		*  @brief
		*    Start the Particle Universe particle system with possible delay
		*/
		void startParticleSystem();

		/**
		*  @brief
		*    Stop the Particle Universe particle system with possible delay
		*
		*  @param[in] fade
		*    If we want to fade out or not
		*/
		void stopParticleSystem(bool fade = false);

		/**
		*  @brief
		*    All particles can be globally disabled if this method is called on all particles
		*/
		void onParticlesGloballyDisabled(bool disabled);

		//[-------------------------------------------------------]
		//[ Update                                                ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Delay update method
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateDelayJob(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Update function to register running time
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateRunningTimeJob(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Update the wrapped Particle Universe particle system instance, can be done in parallel
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateParticleUniverseParticleSystem(const JobArguments& jobArguments);

		/**
		*  @brief
		*    Update the particles component playback, can't be done in parallel
		*/
		void updatePlayback();

		/**
		*  @brief
		*    Register particles component to manager update
		*/
		void registerToManagerUpdate();

		/**
		*  @brief
		*    Unregister particles component from manager update
		*/
		void unregisterFromManagerUpdate();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			RESTART_ON_ACTIVATION,		///< Set it to restart the particles animation on activation
			REPEAT_ANIMATION,			///< Set it to repeat the animation from the particles
			SHOW_ANIMATION,				///< Set it to show the animation once
			PLAYBACK_FINISHED,			///< Set when the playback is finished
			OBJECT_RENDERING_SHUTDOWN,	///< Set when the particles rendering stopped because it got out of sight
			GLOBALLY_DISABLED,			///< Set when the particles are globally disabled (only set when component is running)
			UPDATE_REGISTERED			///< Particles component has been registered to be updated
		};
		typedef EnumBitmask<uint8, InternalFlags> InternalFlagSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		AssetProxy	mParticlesAssetProxy;		///< The particles asset to use (e.g. "sample/particles/fire/campfire")
		bool		mTriedLoading;				///< Has loading the particle effect already beeing tried?
		float		mScaleVelocity;				///< Velocity scale
		bool		mKeepLocal;					///< If this property is set to "true", the particles are emitted relative to the system, else "false"
		bool		mTightBoundingBox;			///< Is "true" when the bounding box is wrapped tight around the particle system or "false" when the bounding box is only increasing and doesn't shrink when the particle system shrinks
		bool		mEmitterEnabled;			///< "true" if the particle emitter is enabled, else "false"
		float		mEmitterIntensity;			///< Particle emitter intensity (0..1, none..full)
		float		mIterationInterval;			///< The value that determines in which intervals the system must be the updated
		float		mFixedTimeout;				///< The past time after which the particle system automatically stops (in seconds)
		// Internal only
		ParticleUniverseParticleSystemInstanceManager* mParticleUniverseParticleSystemInstanceManager;
		InternalFlagSet						mInternalFlags;						///< Set of internal flags, see enum "InternalFlags"
		ParticleUniverse::ParticleSystem*	mParticleUniverseParticleSystem;	///< Particle Universe particle system instance, can be a null pointer
		ParticleSystemListener*				mParticleSystemListener;			///< Particle Universe particle system listener instance, can be a null pointer
		float								mOriginalScaleVelocity;				///< Original scale velocity from the Particle Universe particle system instance
		glm::vec3							mOriginalScale;						///< Original scale from the Particle Universe particle system instance
		bool								mTransformChangeSinceLastUpdate;	///< Has the transform been changed since the Particle Universe particle system instance was last updated?
		Ogre::SceneNode*					mOgreSceneNode;						///< OGRE scene node instance, can be a null pointer
		Ogre::MovableObject*				mOgreMovableObject;					///< OGRE movable instance, can be a null pointer
		JobProxy							mUpdateDelayJobProxy;				///< Delay job proxy
		JobProxy							mRunningTimeJobProxy;				///< Job proxy for Running Time update
		Time								mRunningTime;						///< Time the particle is active since start
		Time								mDelayPassed;						///< Delay time passed
		Time								mDelayTime;							///< Delay we want to wait
		Time								mFastForward;						///< Fast forward time
		bool								mFirstVisible;
		bool								mRealTimeParticles;					///< Real time particles are not influenced by game speed
		float								mMaximumRenderingDistance;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/renderer/particles/ParticlesComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ParticlesComponent)
