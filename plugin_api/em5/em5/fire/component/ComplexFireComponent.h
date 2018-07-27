// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/fire/component/FireReceiverComponent.h" // As parent class
#include "em5/fire/component/FireComponent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class DamageComponent;
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
	*    EMERGENCY 5 complex fire component class
	*
	*  @remarks
	*    Is a optional container of fire components.
	*    The complex fire component need one or more entities in the "FireEntityIdArray" slot to work correctly.
	*    It handle the multiple fire components for the entity.
	*    We are able to create entities with more than one fire source inside, having for e.g. a building with one fire on the roof, one from a window and an other one in the entry door.
	*    For avoiding flooding the message system with "StartBurning"-messages from all individual fire component, the complex fire component checks if this message was send before.
	*
	*    Additional it starts swapping fire material energy in case one fire component reaches its limit.
	*    It is also able to ignite/extinguish/destroy all fire component at once
	*/
	class EM5_API_EXPORT ComplexFireComponent : public FireReceiverComponent, public qsf::JobProxyMovableObjectRenderingListener
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class FireComponent;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;		///< "em5::ComplexFireComponent" unique component ID
		static const uint32 FIREENTITIES_ARRAY;	///< Property id for the fire entities array

		static const float MAXIMUM_BALANCING_PERCENT;


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
		inline explicit ComplexFireComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ComplexFireComponent();

		/**
		*  @brief
		*    Collect the energy of all connected child entities and give them nearly the same amount
		*/
		void balancingFireEnergy();

		/**
		*  @brief
		*    Set on random component of the entity on fire; use this as easy access function
		*/
		void startFire(FreeplayEvent* freeplayEvent = nullptr);

		/**
		*  @brief
		*    Set all fire components of the entity on fire; use this as easy access function
		*/
		void startFireOnAllFireComponents(FreeplayEvent* freeplayEvent);

		/**
		*  @brief
		*    Immediately stops the fire of all fire components; use this as easy access function
		*/
		void stopFire();

		/**
		*  @brief
		*    Immediately destroy the entity; use this as easy access function
		*/
		void destroyByFire();

		/**
		*  @brief
		*    Ask all child entities if they are ready for cleanup and return "true" if the cleanup process can start
		*/
		bool isReadyForCleanup() const;

		/**
		*  @brief
		*    Reset all linked fire components
		*/
		void cleanup();

		float getRelativeCombustionEnergy() const;

		void setEnergyPercent(float fraction);

		/**
		*  @brief
		*    Get the complex fire's current fire energy in interval [0.0, 1.0]
		*/
		float getEnergyPercent() const;

		float getTotalDamagePercentage() const;

		/**
		*  @brief
		*    Draw the hit point bar for the entity
		*/
		void updateHitPointBar();

		/**
		*  @brief
		*    Return one burning fire component
		*/
		FireComponent* getBurningFireComponent() const;

		/**
		*  @brief
		*    Adds all assigned fire components to the given container
		*/
		void getAllFireComponents(std::vector<FireComponent*>& fireComponents) const;

		/**
		*  @brief
		*    Return all assigned fire particle IDs
		*/
		const std::vector<uint64>& getAllFireParticleIds() const;

		/**
		*  @brief
		*    Returns "true" if the fire component is burnable at all
		*/
		bool isBurnable() const;

		void updateParticlesVisibility();


	//[-------------------------------------------------------]
	//[ Public virtual em5::FireReceiver methods              ]
	//[-------------------------------------------------------]
	public:
		virtual bool canCountedForExtinguishAchievement() const override;
		virtual void setCountedForExtinguishAchievement(bool canCounted) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::FireReceiver methods              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return "true" if the total amount of material energy is used; in this case the fire is over
		*/
		inline virtual bool isBurned() const override;

		/**
		*  @brief
		*    Return true if the fire is active, in this case the fire will charge by themself
		*/
		inline virtual bool isBurning() const override;

		/**
		*  @brief
		*    Adds the entity and its cooling/extinguish energy
		*
		*  @param[in] entityId
		*    Id of the entity which applies cooling energy to the fire receiver component
		*  @param[in] extinguishEnergy
		*    The amount of extinguish energy applied (as positive value)
		*  @param[in] coolingEnergy
		*    The amount of cooling energy applied (as positive value)
		*
		*  @remarks
		*    This method delegates the energy to the child fire components in this way
		*     - All burning fire components get the exinguishEnergy/<burning firecomponents>
		*     - All not burning fire components get full coolingEnergy
		*/
		virtual void addCoolingEnergySource(uint64 entityId, float extinguishEnergy, float coolingEnergy) override;

		/**
		*  @brief
		*    Removes the entity and its cooling/extinguish energy
		*
		*  @param[in] entityId
		*    ID of the entity which stopped applying cooling energy to the fire component
		*
		*  @note
		*    - This method delegates it to the child fire components
		*/
		virtual void removeCoolingEnergySource(uint64 entityId) override;


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
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		// Job updates
		void updateDamageComponent();

		/**
		*  @brief
		*    Return "true" if balancing process is possible and necessary. Return important values via out parameters.
		*
		*  @param[out] totalEnergy
		*    The sum of energy from all fitting fire component, if value is > than 0.0f it makes a balance necessary
		*  @param[in] fireComponents
		*    All fire components with energy for balancing
		*/
		bool checkForBalancingEnergy(float& totalEnergy, const std::vector<FireComponent*>& fireComponents) const;

		void rebuildFireComponentArray();

		// This method must be called after the fire component array is build/updated
		void assignFireParticlesToFireComponents();
		void updateComplexFireVisualization();	// TODO(co) Complex fire visualization test so I'm able to test the visual aspect

		// State change
		void recheckFireState();
		void onStartedBurning();
		void onStoppedBurning();

		// Called by em5::FireComponent
		void onFireComponentStartedBurning(FireComponent& fireComponent);
		void onFireComponentStoppedBurning(FireComponent& fireComponent);

		// Gets called when a fire/fireparticle entity slot changes
		void onFireAndFireParticleEntitiesSlotChanged();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<uint64> EntityIdArray;					///< Array of entity IDs
		typedef std::vector<FireComponent*> FireComponentArray;		///< Array of fire components


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy				mSimulationJobProxy;	///< Regular simulation job proxy; for updates once a game tick
		EntityIdArray				mEntityIds;
		EntityIdArray				mFireParticleEntityIds;
		FireComponent::FireState	mFireState;
		FireComponentArray			mFireComponents;
		qsf::WeakPtr<DamageComponent> mDamageComponent;
		size_t						mNumFireObjects;

		// Fire bar
		qsf::DebugDrawProxy		mHitpointbarProxy;
		uint32					mLastShownHitpointValue;
		float					mTotalDamage;

		// Helper member
		bool					mPrintedNoFireComponentsWarning;			///< Indicates if we have already printed a warning that this component has no fire components assigned (Is only relevant ingame);
		qsf::JobProxy			mOneShotUpdateParticleAssignmentJobProxy;	///< Jobproxy to do a one shot update of the fire particles assignment when the fire/fireparticle entities slots changes


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more

	public:
		QSF_CAMP_ARRAY_INTERFACE_CLASS_WITH_VARIABLE(EntityIdArrayInterface, ComplexFireComponent, std::vector<uint64>, uint64);
		EntityIdArrayInterface FireEntityIdArray;	///< Array of the fire entity IDs
		EntityIdArrayInterface FireParticleIdArray; ///< Array of the fire particle entity IDs


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/fire/component/ComplexFireComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::ComplexFireComponent)
QSF_CAMP_ARRAY_MAPPER_DEFINITION(em5::ComplexFireComponent::EntityIdArrayInterface)
