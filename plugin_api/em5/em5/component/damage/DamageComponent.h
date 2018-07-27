// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/component/damage/DamageType.h"

#include <qsf/renderer/map/JobProxyMovableObjectRenderingListener.h>
#include <qsf/asset/AssetSystemTypes.h>
#include <qsf/component/Component.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireComponent;
	class FreeplayEvent;
}
namespace qsf
{
	class MeshComponent;
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
	*    EMERGENCY 5 damage component class
	*/
	class EM5_API_EXPORT DamageComponent : public qsf::Component, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FireComponent;			// Needs to call "setFireLifePercentage" and "onExplosion"
	friend class ComplexFireComponent;	// Needs to call "setFireLifePercentage"


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::DamageComponent" unique component ID
		static const uint32 DAMAGE;				///< "Damage" unique class property ID inside the class
		static const uint32 DAMAGE_STATE;		///< "DamageState" unique class property ID inside the class
		static const uint32 DAMAGE_SIMULATION;	///< "DamageSimulation" unique class property ID inside the class

		// Damage state (basic definition for all sub-classes)
		enum DamageState
		{
			DAMAGESTATE_INTACT,
			DAMAGESTATE_DAMAGE_LEVEL1,
			DAMAGESTATE_DAMAGE_LEVEL2,
			DAMAGESTATE_DESTROYED,
			DAMAGESTATE_INVALID		// Don't export this via camp, this value is only used internal to signal, that the damage state doesn't matter (e.g. in the fire component when checking when a particle should be active)
		};


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
		inline explicit DamageComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~DamageComponent();

		inline DamageState getDamageState() const;
		void setDamageState(DamageState damageState);

		inline float getFireGlowIntensity() const;
		inline void setFireGlowIntensity(float fireGlowIntensity);

		inline bool getDamageSimulation() const;
		inline void setDamageSimulation(bool damageSimulation);

		//[-------------------------------------------------------]
		//[ Fire                                                  ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Helper function to retrieve a burning FireComponent with the highest energy
		*/
		const FireComponent* getBurningFireComponent() const;

		inline float getFireLifePercentage() const;

		/**
		*  @brief
		*    Show the explosion fire particles without doing any damage
		*/
		void showExplosionFireParticles();

		/**
		*  @brief
		*    Set if we want to block the fire from being extinguished
		*/
		void setFireExtinguishBlocked(bool blocked);


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	public:
		inline virtual bool isIntact() const;
		inline virtual bool isDamaged() const;
		inline virtual bool isDamagedLevel1() const;
		inline virtual bool isDamagedLevel2() const;
		inline virtual bool isDestroyed() const;

		inline virtual bool isBurning() const;

		virtual void receiveDamage(float amount, damagetype::DamageType damageType, qsf::Component* sender = nullptr);
		virtual bool checkReceiveDamage(damagetype::DamageType damageType, qsf::Component* sender = nullptr);

		inline virtual bool isReadyForCleanup() const;
		virtual void cleanup();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::Component methods                 ]
	//[-------------------------------------------------------]
	public:
		virtual void onSetSimulating(bool simulating) override;
		virtual void serialize(qsf::BinarySerializer& serializer) override;

		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup() override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::DamageComponent methods        ]
	//[-------------------------------------------------------]
	protected:
		inline virtual void onSetDamageState(DamageState damageState);
		inline virtual void onSetIntact();
		inline virtual void onSetDamagedLevel1();
		inline virtual void onSetDamagedLevel2();
		inline virtual void onSetDestroyed();

		inline virtual void onSetFireLifePercentage(float fireLifePercentage);
		virtual void onFireStarted(FreeplayEvent* freeplayEvent);
		virtual void onFireStopped();

		///< In case the fire component is not a null pointer, only explosions of that fire component explode, otherwise explode all
		inline virtual void onExplosion(FireComponent* fireComponent);

		inline virtual void onFireDamageReceived(float amount, qsf::Component* sender, bool damageJustStarted);

		virtual void setCustomParametersForFireGlowAndSmutOverlay(const qsf::MeshComponent& meshComponent);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::MovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual qsf::Component* getMovableObjectRenderingListenerComponent() override;
		virtual void onObjectRenderingStartup() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::JobProxyMovableObjectRenderingListener methods ]
	//[-------------------------------------------------------]
	protected:
		virtual void onUpdateMovableObjectRenderingJob(const qsf::JobArguments& jobArguments) override;


	//[-------------------------------------------------------]
	//[ Protected static methods                              ]
	//[-------------------------------------------------------]
	protected:
		static void exchangeDamageMaterials(qsf::Entity& entity, bool setDamaged);
		static qsf::GlobalAssetId getDamageMaterialName(qsf::GlobalAssetId originalMaterialGlobalAssetId);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void setFireStateIntact();
		void setFireStateDestroyed();

		// For (complex) fire component friends
		void setFireLifePercentage(float fireLifePercentage);

		void updateTintableMesh();

		// For fire glow and smut
		void setupFireFadeAnimation();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		float mPeakFireEnergyPercent;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateFireFadeAnimation(const qsf::JobArguments& jobArguments);
		void setCustomParametersForFireGlowAndSmutOverlay();
		void internalDamageStateUpdated(bool isStartup = false);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		DamageState	mDamageState;			///< Current damage state
		float		mFireGlowIntensity;
		// Internal only
		float		mFireLifePercentage;	///< Fire material life percentage in [0.0f, 1.0f], which is the general damage value in many cases (except VehicleComponent sub-class)
		bool		mIsDamageSimulationActive;
		qsf::Time	mLastReceivedFireDamageTimestamp;
		float		mEmissiveMapStrength;

		// Job proxies for regular update
		qsf::JobProxy mFireFadeAnimationJobProxy;	///< Regular animation job proxy; for updates once a frame


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/component/damage/DamageComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::DamageComponent)
QSF_CAMP_TYPE_NONCOPYABLE(em5::DamageComponent::DamageState)
