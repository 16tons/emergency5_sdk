// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ComplexFireComponent::ComplexFireComponent(qsf::Prototype* prototype) :
		FireReceiverComponent(prototype),
		mFireState(FireComponent::FIRESTATE_NOFIRE),
		mNumFireObjects(0),
		mTotalDamage(0.0f),
		mLastShownHitpointValue(qsf::getUninitialized<uint32>()),
		mPrintedNoFireComponentsWarning(false),
		FireEntityIdArray(*this, mEntityIds),
		FireParticleIdArray(*this, mFireParticleEntityIds)
	{
		// Nothing to do in here
	}

	inline ComplexFireComponent::~ComplexFireComponent()
	{
		// Nothing to do in here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::FireReceiver methods              ]
	//[-------------------------------------------------------]
	inline bool ComplexFireComponent::isBurning() const
	{
		return FireComponent::isBurningFireState(mFireState);
	}

	inline bool ComplexFireComponent::isBurned() const
	{
		return (mFireState == FireComponent::FIRESTATE_DESTROYED || mFireComponents.empty());
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t ComplexFireComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& ComplexFireComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void ComplexFireComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
		// Array slot has changed update the fire particle assignments
		// This is needed so that inside the editor the particles are controlled by the fire component state
		getOwner().onFireAndFireParticleEntitiesSlotChanged();
	}

	inline void ComplexFireComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
		// Array slot has changed update the fire particle assignments
		// This is needed so that inside the editor the particles are controlled by the fire component state
		getOwner().onFireAndFireParticleEntitiesSlotChanged();
	}

	inline void ComplexFireComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
		// Array slot has changed update the fire particle assignments
		// This is needed so that inside the editor the particles are controlled by the fire component state
		getOwner().onFireAndFireParticleEntitiesSlotChanged();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
