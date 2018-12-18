// Copyright (C) 2012-2018 Promotion Software GmbH

namespace em5
{
	inline InhabitantComponent::InhabitantComponent(qsf::Prototype* prototype) :
		Component(prototype),
		mActionPriority(0),
		mDestinationEntityId(qsf::getUninitialized<uint64>()),
		mHomeEntityId(qsf::getUninitialized<uint64>()),
		mInhabitantMode(UNDEFINED)
	{
	}

	inline InhabitantComponent::~InhabitantComponent()
	{
	}

	inline uint32 InhabitantComponent::getActionPriority() const
	{
		return mActionPriority;
	}

	inline void InhabitantComponent::setActionPriority(uint32 priority)
	{
		assignAndPromoteChange(mActionPriority, priority, ACTION_PRIORITY);
	}

	inline uint64 InhabitantComponent::getHomeEntityId() const
	{
		return mHomeEntityId;
	}

	inline void InhabitantComponent::setHomeEntityId(uint64 entityId)
	{
		mHomeEntityId=entityId;
	}

	inline uint64 InhabitantComponent::getDestinationEntityId() const
	{
		return mDestinationEntityId;
	}

	inline void InhabitantComponent::setDestinationEntityId(uint64 entityId)
	{
		assignAndPromoteChange(mDestinationEntityId, entityId, DESTINATION_ENTITY_ID);
	}

	inline InhabitantComponent::InhabitantMode InhabitantComponent::getInhabitantMode() const
	{
		return mInhabitantMode;
	}

}
