// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/component/Component.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FireReceiverComponent;
	class HealthComponent;
	class DamageComponent;
}
namespace qsf
{
	class Time;
	class TransformComponent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Structs                                               ]
	//[-------------------------------------------------------]
	namespace firesimulation
	{
		// This structure is used just as performance optimization, to cache certain values so they don't have to be gathered in each fire system update again
		// TODO(fw): There is quite some more potential in caching, add more data here that is expensive to gather
		struct FireReceiverData
		{
			FireReceiverComponent*	mComponent;			///< The fire receiver component associated with this data instance
			glm::vec3				mPosition;			///< World position of the fire receiver
			bool					mHasPosition;		///< "true" if the position is set at all
		};
	}


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 fire receiver component class
	*
	*  @remarks
	*    A fire receiver is a simplified version of the fire component. Humans for e.g. receive energy via fire, but they handle it themself in there health component.
	*/
	class EM5_API_EXPORT FireReceiverComponent : public qsf::Component
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class FireSystem; // Needed to be able to call the both protected virtual methods within the FireSystem (write back calculated fire simulation values)


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 COMPONENT_ID;	///< "em5::FireReceiverComponent" unique component ID


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
		explicit FireReceiverComponent(qsf::Prototype* prototype);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FireReceiverComponent();

		/**
		*  @brief
		*    Adds the entity and its cooling energy
		*
		*  @param[in] entityId
		*    Id of the entity which applies cooling energy to the fire receiver component
		*  @param[in] extinguishEnergy
		*    The amount of extinguish energy applied (as positive value)
		*  @param[in] coolingEnergy
		*    The amount of cooling energy applied (as positive value)
		*/
		virtual void addCoolingEnergySource(uint64 entityId, float extinguishEnergy, float coolingEnergy);

		/**
		*  @brief
		*    Removes the entity and its cooling energy
		*
		*  @param[in] entityId
		*    Id of the entity which stopped applying cooling energy to the fire component
		*/
		virtual void removeCoolingEnergySource(uint64 entityId);

		/**
		*  @brief
		*    Return true if the fire is active, in this case the fire will charge by themself
		*/
		inline virtual bool isBurning() const;

		/**
		*  @brief
		*    Returns true if the total amount of material energy is used; in this case the fire is over
		*/
		inline virtual bool isBurned() const;

		/**
		*  @brief
		*    Returns the amount of cooling energy which is currently applied to this component
		*
		*  @note
		*    - This value is usually negative
		*/
		float getCoolingEnergy() const;

		/**
		*  @brief
		*     Returns the amount of cooling energy sources that interact with this component
		*/
		uint32 getCoolingEnergySourceCount() const;

		/*
		*  @brief
		*    Returns the IDs of the current cooling source entities
		*
		*  @remarks
		*    Call by reference to outsource the memory operations
		*/
		void getCoolingSources(std::vector<uint64>& coolingSourceIds) const;

		/**
		*  @brief
		*    Returns the owning entity's transform component, in case this component is in simulating state
		*/
		qsf::TransformComponent* getTransformComponent() const;

		//[-------------------------------------------------------]
		//[ Achievement                                           ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Is the current fire component counted for the extinguish fires achievement
		*/
		virtual bool canCountedForExtinguishAchievement() const;

		/**
		*  @brief
		*    Enable or disable counting for the extinguishing fires achievement
		*/
		virtual void setCountedForExtinguishAchievement(bool canCounted);


	//[-------------------------------------------------------]
	//[ Protected em5::FireReceiverComponent methods          ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Updates the registration at the fire system for getting simulation updates and taking part at the fire spread calculation
		*/
		void internalActive(bool simulating, bool active);


	//[-------------------------------------------------------]
	//[ Protected virtual em5::FireReceiverComponent methods  ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Return "true" if the fire receiver is a valid target for the sender, ask the health component for dead state by default
		*/
		virtual bool checkFireReceiver(qsf::Component* sender);

		/**
		*  @brief
		*    Handle the received energy and gives it to the special receiver
		*
		*  @param[in] fireEnergy
		*    Amount of energy which gets received
		*  @param[in] timePassed
		*    Time passed since last call
		*  @param[in] sender
		*    Pointer to the sender of the energy, can be nullptr
		*/
		virtual void receiveFireEnergy(float fireEnergy, const qsf::Time& timePassed, qsf::Component* sender);

		/**
		*  @brief
		*    Update the simulation state of this component
		*
		*  @param[in] timePassed
		*    Time passed since last call
		*/
		virtual void updateSimulation(const qsf::Time& timePassed);


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::Component methods              ]
	//[-------------------------------------------------------]
	protected:
		virtual void onSetSimulating(bool simulating) override;
		virtual void onSetActive(bool active) override;
		virtual void onComponentTransformChange(const qsf::Transform& transform, TransformChangeFlags flags) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		qsf::WeakPtr<qsf::TransformComponent> mTransformComponent;
		qsf::WeakPtr<HealthComponent>		  mHealthComponent;
		qsf::WeakPtr<DamageComponent>		  mDamageComponent;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::container::flat_map<uint64, float> mCoolingEnergySet;	///< Holds a list of entity IDs and their cooling energy applied to the target (per second)
		firesimulation::FireReceiverData*		  mFireReceiverData;	///< Instance is managed by the fire system, not the fire receiver component


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
#include "em5/fire/component/FireReceiverComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::FireReceiverComponent)
