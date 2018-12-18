// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SpawnGroupComponent::SpawnGroupComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mSpawnGroup(qsf::getUninitialized<uint32>())
	{
		// Nothing here
	}

	inline SpawnGroupComponent::~SpawnGroupComponent()
	{
		// Nothing here
	}

	inline uint32 SpawnGroupComponent::getSpawnGroup() const
	{
		return mSpawnGroup;
	}

	inline void SpawnGroupComponent::setSpawnGroup(uint32 group)
	{
		mSpawnGroup = group;
	}

	inline const qsf::NamedIdentifier& SpawnGroupComponent::getSpawnTag() const
	{
		return mSpawnTag;
	}

	inline void SpawnGroupComponent::setSpawnTag(const qsf::NamedIdentifier& tag)
	{
		mSpawnTag = tag;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
