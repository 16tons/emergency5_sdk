// Copyright (C) 2012-2017 Promotion Software GmbH


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
	inline FireParticleOptionsComponent::FireParticleOptionsComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mShowOnLargeFireSate(false),
		mFireLightEntityId(qsf::getUninitialized<uint64>())
	{
		// Initial for all damage level the particle will be shown, but not for explosions
		mDamageLevelFlags.set(DAMAGE_INTACT);
		mDamageLevelFlags.set(DAMAGE_LEVEL1);
		mDamageLevelFlags.set(DAMAGE_LEVEL2);
		mDamageLevelFlags.set(DAMAGE_LEVEL3);
	}

	inline FireParticleOptionsComponent::~FireParticleOptionsComponent()
	{
		// Nothing to do in here
	}

	inline bool FireParticleOptionsComponent::getShowOnIntact() const
	{
		return mDamageLevelFlags.isSet(DAMAGE_INTACT);
	}

	inline void FireParticleOptionsComponent::setShowOnIntact(bool show)
	{
		mDamageLevelFlags.set(DAMAGE_INTACT, show);
	}

	inline bool FireParticleOptionsComponent::getShowOnDamageLevel1() const
	{
		return mDamageLevelFlags.isSet(DAMAGE_LEVEL1);
	}

	inline void FireParticleOptionsComponent::setShowOnDamageLevel1(bool show)
	{
		mDamageLevelFlags.set(DAMAGE_LEVEL1, show);
	}

	inline bool FireParticleOptionsComponent::getShowOnDamageLevel2() const
	{
		return mDamageLevelFlags.isSet(DAMAGE_LEVEL2);
	}

	inline void FireParticleOptionsComponent::setShowOnDamageLevel2(bool show)
	{
		mDamageLevelFlags.set(DAMAGE_LEVEL2, show);
	}

	inline bool FireParticleOptionsComponent::getShowOnDamageLevel3() const
	{
		return mDamageLevelFlags.isSet(DAMAGE_LEVEL3);
	}

	inline void FireParticleOptionsComponent::setShowOnDamageLevel3(bool show)
	{
		mDamageLevelFlags.set(DAMAGE_LEVEL3, show);
	}

	inline bool FireParticleOptionsComponent::getShowOnExplosion() const
	{
		return mDamageLevelFlags.isSet(DAMAGE_EXPLOSION);
	}

	inline void FireParticleOptionsComponent::setShowOnExplosion(bool show)
	{
		mDamageLevelFlags.set(DAMAGE_EXPLOSION, show);
	}

	inline bool FireParticleOptionsComponent::getShowOnLargeFireState() const
	{
		return mShowOnLargeFireSate;
	}

	inline void FireParticleOptionsComponent::setShowOnLargeFireState(bool show)
	{
		mShowOnLargeFireSate = show;
	}

	inline uint64 FireParticleOptionsComponent::getFireLightEntityId() const
	{
		return mFireLightEntityId;
	}

	inline void FireParticleOptionsComponent::setFireLightEntityId(uint64 fireLightEntity)
	{
		mFireLightEntityId = fireLightEntity;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
