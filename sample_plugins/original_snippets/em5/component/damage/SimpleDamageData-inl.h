// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <camp/arraymapper.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SimpleDamageData::SimpleDamageData() :
		DamagedLevel1Particles(*this, mDamagedLevel1ParticleIds),
		DamagedLevel2Particles(*this, mDamagedLevel2ParticleIds),
		DamagedLevel3Particles(*this, mDamagedLevel3ParticleIds),
		DestroyedParticles(*this, mDestroyedParticleIds)
	{
		// Nothing to do in here
	}

	inline SimpleDamageData::~SimpleDamageData()
	{
		// Nothing to do in here
	}

	inline const qsf::AssetProxy& SimpleDamageData::getIntactMesh() const
	{
		return mIntactMesh;
	}

	inline void SimpleDamageData::setIntactMesh(const qsf::AssetProxy& mesh)
	{
		mIntactMesh = mesh;
	}

	inline const qsf::AssetProxy& SimpleDamageData::getDamagedMesh() const
	{
		return mDamagedMesh;
	}

	inline void SimpleDamageData::setDamagedMesh(const qsf::AssetProxy& mesh)
	{
		mDamagedMesh = mesh;
	}

	inline const qsf::AssetProxy& SimpleDamageData::getDestroyedMesh() const
	{
		return mDestroyedMesh;
	}

	inline void SimpleDamageData::setDestroyedMesh(const qsf::AssetProxy& mesh)
	{
		mDestroyedMesh = mesh;
	}

	inline const qsf::AssetProxy& SimpleDamageData::getSmutTexture() const
	{
		return mSmutTexture;
	}

	inline void SimpleDamageData::setSmutTexture(const qsf::AssetProxy& smutTexture)
	{
		mSmutTexture = smutTexture;
	}

	inline const qsf::AssetProxy& SimpleDamageData::getDamageTexture() const
	{
		return mDamageTexture;
	}

	inline void SimpleDamageData::setDamageTexture(const qsf::AssetProxy& damageTexture)
	{
		mDamageTexture = damageTexture;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t SimpleDamageData::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& SimpleDamageData::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void SimpleDamageData::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void SimpleDamageData::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void SimpleDamageData::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
