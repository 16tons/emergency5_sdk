// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>

#include <camp/arraymapper.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline EboxComponent::EboxComponent(qsf::Prototype* prototype) :
		ObjectDamageComponent(prototype),
		mFlashWaitTime(5.0f, 10.0f),
		mSparksWaitTime(5.0f, 10.0f),
		mIsDefect(false),
		mGapeChance(0.0f),
		mReactionRadius(0.0f),
		mMaxInjuredPersons(1),
		mCurrentInjuredPersons(0),
		mSecondsToNextFlash(qsf::Time::ZERO),
		mSecondsToNextSparks(qsf::Time::ZERO),
		FlashParticlesArray(*this, mFlashParticlesIds),
		SparksParticlesArray(*this, mSparksParticlesIds)
	{
		// Nothing here
	}

	inline EboxComponent::~EboxComponent()
	{
		// Nothing to do in here
	}

	inline bool EboxComponent::isDefect() const
	{
		return mIsDefect;
	}

	inline const qsf::AssetProxy& EboxComponent::getDefectMesh() const
	{
		return mDamageMesh;
	}

	inline void EboxComponent::setDefectMesh(const qsf::AssetProxy& mesh)
	{
		mDamageMesh = mesh;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::DamageComponent methods           ]
	//[-------------------------------------------------------]
	inline bool EboxComponent::isReadyForCleanup() const
	{
		return isDamaged() ? true : ObjectDamageComponent::isReadyForCleanup();
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t EboxComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& EboxComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void EboxComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void EboxComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void EboxComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
