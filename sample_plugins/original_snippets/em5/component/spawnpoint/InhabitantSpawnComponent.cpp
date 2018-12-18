// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/PrecompiledHeader.h"
#include "em5/component/spawnpoint/InhabitantSpawnComponent.h"
#include "em5/component/person/InhabitantComponent.h"

#include <qsf/map/Entity.h>
#include <qsf/map/Map.h>
#include <qsf/math/Random.h>


namespace em5
{
	const uint32 InhabitantSpawnComponent::COMPONENT_ID = qsf::StringHash("em5::InhabitantSpawnComponent");
	const uint32 InhabitantSpawnComponent::PERSON = qsf::StringHash("Person");
	const uint32 InhabitantSpawnComponent::DESTINATION_ENTITY_ID = qsf::StringHash("DestinationEntityId");
	const uint32 InhabitantSpawnComponent::TIME_BEGIN = qsf::StringHash("TimeBegin");
	const uint32 InhabitantSpawnComponent::TIME_END = qsf::StringHash("TimeEnd");

	void InhabitantSpawnComponent::serialize(qsf::BinarySerializer& serializer)
	{
		SpawnPointComponent::serialize(serializer);
		serializer & mDestinationEntityId;
		serializer & mTimeBegin;
		serializer & mTimeEnd;
		serializer & mTime;
		uint64 inhabitantId(qsf::getUninitialized<uint64>());
		if (serializer.isReading())
		{
			mInhabitant.clear();
			serializer & inhabitantId;
			if (qsf::isInitialized(inhabitantId))
				mInhabitant=getEntity().getMap().getEntityById(inhabitantId);
		}
		else
		{
			if (mInhabitant.valid())
				inhabitantId=mInhabitant->getId();
			serializer & inhabitantId;
		}
	}

	void InhabitantSpawnComponent::updateSpawnTime()
	{
		if (mTimeBegin<=mTimeEnd)
			mTime=qsf::Random::getRandomInt(mTimeBegin, mTimeEnd);
		else
		{
			const int minutesPerDay=60*24;
			mTime=qsf::Random::getRandomInt(mTimeBegin, mTimeEnd+minutesPerDay);
			if (mTime>=minutesPerDay)
				mTime-=minutesPerDay;
		}
	}

	bool InhabitantSpawnComponent::spawnInhabitant()
	{
		if (!mInhabitant.valid())
		{
			mInhabitant=spawnRandom();
			if (mInhabitant.valid())
			{
				InhabitantComponent* inhabitantComponent=mInhabitant->getOrCreateComponent<InhabitantComponent>();
				if (inhabitantComponent)
				{
					inhabitantComponent->setHomeEntityId(getEntityId());
					if (!inhabitantComponent->getDestinationEntityId())
						inhabitantComponent->setDestinationEntityId(mDestinationEntityId);
					inhabitantComponent->setInhabitantMode(InhabitantComponent::GOTO_DESTINATION);
				}
				return true;
			}
		}
		return false;
	}

}
