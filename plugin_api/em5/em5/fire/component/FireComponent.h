// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/fire/component/FireReceiverComponent.h"
#include "em5/component/damage/DamageComponent.h"	// For the DamageState enum

#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/math/EnumBitmask.h>
#include <qsf/audio/AudioProxy.h>

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireMaterial;
	class FreeplayEvent;
	class GameDebugGroup;
	class ComplexFireComponent;
	namespace multiplayer
	{
		class FireData;
	}
}
namespace qsf
{
	class TransformComponent;
	class CameraComponent;
	class MessageParameters;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 fire component class
	*
	*  @note
	*    - Most ingame buildings, vehicles, vegetation, objects (cans, garbage) have fire components
	*/
	class EM5_API_EXPORT FireComponent : public FireReceiverComponent
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ComplexFireComponent;
	friend class FireSystem;

	// TODO(sw) This is only needed for multiplay and there only on client side
	//			It would be better, when the simulation and pure visually updates are split into separate methods
	//			The pure visually updates could then be called cyclic on the multiplayer client (via an update job) to update the visually aspects
	friend class multiplayer::FireData;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::FireComponent" unique component ID
		static const uint32 HARD_RADIUS;
		static const uint32 SOFT_RADIUS;

		enum FireState
		{
			FIRESTATE_NOFIRE,		///< Entity is currently not burning (but may still be damaged by a former fire)
			FIRESTATE_SMALLFIRE,	///< Entity is burning with a small fire, i.e. it's not burning very hot yet
			FIRESTATE_LARGEFIRE,	///< Entity is burning with a large fire, i.e. it's burning relatively hot now
			FIRESTATE_DESTROYED,	///< Entity is not burning, but completely destroyed by fire; it can't start burning again because there's nothing left for the fire
			_NUM_FIRESTATES
		};

		enum FireAudioState
		{
			FIREAUDIOSTATE_NOFIRE,
			FIREAUDIOSTATE_SMALLFIRE,
			FIREAUDIOSTATE_MEDUIMFIRE,
			FIREAUDIOSTATE_LARGEFIRE,
			_NUM_FIREAUDIOSTATES
		};


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static bool getAllFireComponents(qsf::Entity& entity, std::vector<FireComponent*>& outFireComponents);
		inline static bool isBurningFireState(FireState fireState);


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
		explicit FireComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FireComponent();

		/**
		*  @brief
		*    Return the main entity of this fire component
		*
		*  @return
		*    Returns the parent entity of this fire component or the entity of this component if there is none
		*/
		qsf::Entity& getMainEntity() const;

		/**
		*  @brief
		*    Return the complex fire component this belongs to
		*
		*  @return
		*    The parent complex fire component if this is just a child fire component, or null pointer if this is itself the top-level fire component
		*/
		inline ComplexFireComponent* getComplexFireComponent() const;

		/**
		*  @brief
		*    Show the particles which are edited as show on explosion
		*    Usually no need to call this manually, but needed by the DamageComonent::onExplosion derivatives
		*/
		void showOnExplosionFireParticles();

		//[-------------------------------------------------------]
		//[ Properties                                            ]
		//[-------------------------------------------------------]
		float getRelativeCombustionEnergy() const;

		/**
		*  @brief
		*    Return the energy of the fire
		*
		*  @return
		*    Returns the current energy value
		*/
		inline float getEnergy() const;

		/**
		*  @brief
		*    Set the energy of the fire
		*
		*  @param[in] energy
		*    The current energy value
		*/
		inline void setEnergy(float energy);

		void setEnergyPercent(float fraction);

		float getMaxEnergyValue() const;

		/**
		*  @brief
		*    Get the fire's current fire energy in interval [0.0, 1.0]
		*/
		float getEnergyPercent() const;

		/**
		*  @brief
		*    Return the amount of energy the fire has damage the entity
		*
		*  @return
		*    The total damage value
		*/
		inline float getTotalDamage() const;

		/**
		*  @brief
		*    Set the total damage value of the entity, if higher than getLife() of "em5::FireMaterial", entity is destroyed
		*
		*  @param[in] damage
		*    The total damage to the entity
		*/
		inline void setTotalDamage(float damage);

		float getTotalDamagePercentage() const;

		/**
		*  @brief
		*    Return the maximum life of the fire material
		*/
		float getMaximumLife() const;

		/**
		*  @brief
		*    Return the current fire state
		*/
		inline FireState getFireState() const;

		/**
		*  @brief
		*     Set the current fire state. Show or hide the necessary graphics, like particles or building destruction.
		*
		*  @note
		*    This is reflected to camp export
		*/
		void setFireState(FireState fireState);

		/**
		*  @brief
		*     Set fire state by event
		*/
		void setFireStateByEvent(FireState fireState, FreeplayEvent* freeplayEvent = nullptr);

		/**
		*  @brief
		*    Return the name of the fire material asset to use
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		const std::string& getFireMaterialName() const;

		/**
		*  @brief
		*    Set the name of the fire material asset to use
		*
		*  @note
		*    - Connected to the CAMP reflection system
		*/
		void setFireMaterialByName(const std::string& fireMaterialName);

		/**
		*  @brief
		*    Return the hard fire radius in meters; objects inside the hard radius take full fire damage
		*/
		inline float getHardRadius() const;

		/**
		*  @brief
		*    Set the hard fire radius
		*
		*  @param[in] hardRadius
		*    Hard radius from the fire child in meters
		*/
		inline void setHardRadius(float hardRadius);

		/**
		*  @brief
		*    Return the soft fire radius in meters; objects inside the soft radius take less damage fire damage
		*/
		inline float getSoftRadius() const;

		/**
		*  @brief
		*   Set the soft fire radius
		*
		*  @param[in] softRadius
		*   Soft radius from the fire child in meters
		*/
		inline void setSoftRadius(float softRadius);

		/**
		*  @brief
		*   Returns if fire sounds are playing for this component
		*/
		inline bool getPlayAudio();

		/**
		*  @brief
		*   Set if fire sounds are playing for this component
		*/
		inline void setPlayAudio(bool playAudio);

		//[-------------------------------------------------------]
		//[ State query                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return if the entity is still intact, i.e. has not burned before
		*/
		inline bool isIntact() const;

		/**
		*  @brief
		*    Return "true" if the fire is active, in this case the fire will charge by themselves
		*/
		inline virtual bool isBurning() const override;

		/**
		*  @brief
		*    Returns "true" if the total amount of material energy is used; in this case the fire is over
		*/
		inline virtual bool isBurned() const override;

		/**
		*  @brief
		*    Returns "true" if the fire component is burnable at all
		*/
		inline bool isBurnable() const;

		/**
		*  @brief
		*    Returns the blast force value of the fire material
		*  @note
		*    This method is only needed for the fire spread calculation done via FireSpreadCalculation
		*/
		float getExplosionBlastForce() const;

		/**
		*  @brief
		*    Returns the blast radius of the fire material
		*  @note
		*    This method is only needed for the fire spread calculation done via FireSpreadCalculation
		*/
		float getExplosionBlastRadius() const;

		/**
		*  @brief
		*    Returns the fire resistance of the fire material
		*  @note
		*    This method is only needed for the fire spread calculation done via FireSpreadCalculation
		*/
		float getFireResistance() const;

		/**
		*  @brief
		*    Returns the received energy factor
		*/
		float getReceivedEnergyFactor() const;

		/**
		*  @brief
		*    Set the received energy factor
		*/
		void setReceivedEnergyFactor(float factor);

		//[-------------------------------------------------------]
		//[ Cleanup                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return true if the component is not on fire and it doesn't plan to start a fire. Must get fire damage one time.
		*/
		bool isReadyForCleanup() const;

		/**
		*  @brief
		*    Reset the fire component so it will burn again
		*/
		void cleanup();

		//[-------------------------------------------------------]
		//[ Fire control                                          ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Set the entity on fire. Use this as easy access function, it's also possible to set the fire state directly
		*/
		void startFire(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Immediately stops the fire. Use this as easy access function, it's also possible to set the fire state directly
		*/
		void stopFire();

		/**
		*  @brief
		*    Immediately destroy the entity. Use this as easy access function, it's also possible to set the fire state directly
		*/
		void destroyByFire();

		/**
		*  @brief
		*    Immediately destroy the entity, with an explosion if the material is edited with one. Use this as easy access function, it's also possible to set the fire state directly
		*/
		void triggerExplosion();

		/**
		*  @brief
		*    Hide the particle of the linked entities
		*
		*  @note
		*    This is called by the fire system every time we save the map. We use this to avoid performance issues in editor.
		*/
		void hideParticleChildrenAtMapSave();

		/**
		*  @brief
		*    Give fire damage to the component, use this only for debug reasons
		*/
		void forceFireDamage(float fireDamage);

		inline void updateParticlesVisibility() { setParticlesActive(); }

		/*
		*  @brief
		*    Get/Set if extinguishing of the fire is blocked or not
		*/
		inline bool isExtinguishBlocked() const;
		inline void setExtinguishBlocked(bool extinguishBlocked);

		//[-------------------------------------------------------]
		//[ Debug                                                 ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Activate/Deactivate displaying the fire radii (only needed for the editor)
		*/
		void setShowFireRadii(bool showFireRadii);

		/**
		*  @brief
		*    Adds debug draw requests for the fire radii to the given debug draw proxy
		*/
		void addFireRadiiDebugDrawRequests(qsf::DebugDrawProxy& debugDrawProxy);

		//[-------------------------------------------------------]
		//[ Tutorial                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sets a limit for the fire energy and fire damage (Percentage value -> When limit reached any further extinguishing doesn't have any effect or any additional damage by fire is ignored)
		*
		*  @note
		*    - Currently this is only needed for the tutorial
		*/
		void setFireEnergyAndDamageRestrictionPercentage(float limitPercentage);


	//[-------------------------------------------------------]
	//[ Public virtual em5::FireReceiver methods              ]
	//[-------------------------------------------------------]
	public:
		virtual bool canCountedForExtinguishAchievement() const override;
		virtual void setCountedForExtinguishAchievement(bool canCounted) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::FireReceiver methods           ]
	//[-------------------------------------------------------]
	protected:
		virtual bool checkFireReceiver(qsf::Component* senderComponent) override;
		virtual void receiveFireEnergy(float fireEnergy, const qsf::Time& timePassed, qsf::Component* senderComponent) override;
		virtual void updateSimulation(const qsf::Time& timePassed) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetActive(bool active) override;
		virtual void onSetSimulating(bool simulating) override;
		virtual bool onStartup() override;
		virtual void onShutdown() override;
		inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
		virtual void onComponentPropertyChange(const qsf::Component& component, uint32 propertyId) override;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static void onObjectStartedBurning(qsf::Entity& entity, qsf::Component* sender);
		static void onObjectStoppedBurning(qsf::Entity& entity, bool destroyed, bool isCleanup = false);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			FLAG_INTACT		///< Flag indicates if entity was burning at least one time
		};
		typedef qsf::EnumBitmask<uint8, InternalFlags> InternalFlagSet;

		typedef std::vector<uint64> EntityIdArray;		///< Array of entity IDs


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateReceivedEnergy(const qsf::Time& timePassed);
		void updateBurning(const qsf::Time& timePassed);
		void updateFireStateChanges();
		void updateBalanceEnergy();
		void updateHitPointBar();
		void updateDamageComponent();

		// State change
		void onFireStateChange(FireState newFireState, FireState oldFireState, FreeplayEvent* freeplayEvent);
		void onStartedBurning(FreeplayEvent* freeplayEvent);
		void onStoppedBurning();

		// Helper
		FreeplayEvent* getSenderEvent();
		void loadFireMaterial();
		void onFadeInLight(const qsf::MessageParameters& parameters);

		// Particles
		void setParticlesActive();
		void stopAllParticles();
		void setParticlesActiveInternal(DamageComponent::DamageState damageLevel);
		void detectEarlyParticleStoping();

		// Exploding flag
		inline bool isExploding() const;
		inline void setIsExploding(bool isExploding);

		inline bool internalIsBurning() const;
		inline bool internalIsBurned() const;

		// Sensor and ReactionComponent Handling
		void setSensorAndReactionComponent(bool active);

		// Audio
		void updateAudio();

		// Debug
		void showDebug();
		void onPreCompositorWorkspaceUpdate(const qsf::CameraComponent& cameraComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to CAMP
		FireState				mFireState;				///< The current fire state (e.g. FIRESTATE_LARGEFIRE)
		float					mHardRadius;			///< Entities in this radius receive 100% fire damage
		float					mSoftRadius;			///< Entities in this radius receive less fire damage
		float					mFireEnergy;			///< Current energy of the fire, can't get higher than getMaxEnergyValue() from the fire material
		float					mTotalDamage;			///< Total used energy of the fire, can't get higher than getLifeValue() from the fire material, if this point is reached, the object gets destroyed
		InternalFlagSet			mInternalFlags;			///< Set of internal flags, see enum "InternalFlags"
		EntityIdArray			mFireParticlesIds;		///< Array of particles entity IDs

		// Internal
		GameDebugGroup&			mGameDebugGroup;					///< Cached game debug group instance, do not destroy the instance
		std::string				mFireMaterialName;					///< The name of the fire material to use
		FireMaterial*			mFireMaterial;						///< The fire material to use, can be a null pointer
		float					mCombustionEnergy;					///< Starts with getCombustionValue() from FireMaterial, if zero, fire must start
		float					mReceivedEnergy;					///< Amount of energy received by other entities, set to zero every simulation update
		qsf::Component*			mSenderComponent;					///< Component receives fire from the sender component, need it for sharing event ID
		ComplexFireComponent*	mComplexFireComponent;				///< The complex fire component this fire component belongs to; may be a null pointer, do not destroy the instance
		bool					mIsExploding;						///< Indicates if the component has exploded due too much fire energy. This is only needed for the fire spread calculation done outside of the component
		bool					mShowFireRadii;						///< Indicates if the fire radii should be shown or not (only needed for the editor)
		float					mLowerFireEnergyLimitPercentage;	///< Defines the lower limit how the fire energy can be decreased when being extinguished  (Note: Currently only needed for the tutorial to avoid that the entity either gets extinguished or gets burned out)
		float					mUpperFireDamageLimitPercentage;	///< Defines the upper limit how of the damaged received by the fire (Note: Currently only needed for the tutorial to avoid that the entity either gets extinguished or gets burned out)
		glm::vec3				mFireParticlesCenter[2];
		float					mFireParticlesRadius[2];
		qsf::MessageProxy		mLightDelayMessageProxy;
		bool					mIsInCleanupProcess;				///< "true" while we are in a cleanup process
		float					mReceivedEnergyFactor;				///< Factor multiplied on incoming fire energy, usually 1.0f
		bool					mExtinguishBlocked;
		bool					mWasAlwaysInactive;					///< "true" is fire component was edited as inactive
		boost::container::flat_set<uint64> mDelayedLightIds;

		// Audio
		bool					mPlayAudio;
		FireAudioState			mFireAudioState;
		uint64					mAmbientAudioSourceHandle;

		// Debugging
		//  -> Actually only for internal builds (additional check to find erroneous behavior), but we need to make sure end-user SDK is the same with or without ENDUSER define
		qsf::DebugDrawProxy		mDebugDrawProxy;
		qsf::DebugDrawProxy		mHitpointbarProxy;
		uint32					mLastShownHitpointValue;

		// Achievement
		bool mCanCountedForExtinguishAchievement;

		boost::signals2::connection mPreCompositorWorkspaceUpdateConnection;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, FireComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface FireParticlesArray;	///< Array of the particles for the fire


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/fire/component/FireComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FireComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::FireComponent::FireState)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::FireComponent::EntityIdArrayInterface)
