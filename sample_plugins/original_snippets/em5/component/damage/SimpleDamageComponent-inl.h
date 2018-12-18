// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SimpleDamageComponent::SimpleDamageComponent(qsf::Prototype* prototype) :
		DamageComponent(prototype)
	{
		// Nothing here
	}

	inline SimpleDamageComponent::~SimpleDamageComponent()
	{
		// Nothing here
	}

	inline const qsf::AssetProxy& SimpleDamageComponent::getIntactMesh() const
	{
		return mDamageData.getIntactMesh();
	}

	inline void SimpleDamageComponent::setIntactMesh(const qsf::AssetProxy& mesh)
	{
		mDamageData.setIntactMesh(mesh);
	}

	inline const qsf::AssetProxy& SimpleDamageComponent::getDamagedMesh() const
	{
		return mDamageData.getDamagedMesh();
	}

	inline void SimpleDamageComponent::setDamagedMesh(const qsf::AssetProxy& mesh)
	{
		mDamageData.setDamagedMesh(mesh);
	}

	inline const qsf::AssetProxy& SimpleDamageComponent::getDestroyedMesh() const
	{
		return mDamageData.getDestroyedMesh();
	}

	inline void SimpleDamageComponent::setDestroyedMesh(const qsf::AssetProxy& mesh)
	{
		mDamageData.setDestroyedMesh(mesh);
	}

	inline const qsf::AssetProxy& SimpleDamageComponent::getSmutTexture() const
	{
		return mDamageData.getSmutTexture();
	}

	inline void SimpleDamageComponent::setSmutTexture(const qsf::AssetProxy& smutTexture)
	{
		mDamageData.setSmutTexture(smutTexture);
	}

	inline const qsf::AssetProxy& SimpleDamageComponent::getDamageTexture() const
	{
		return mDamageData.getDamageTexture();
	}

	inline void SimpleDamageComponent::setDamageTexture(const qsf::AssetProxy& damageTexture)
	{
		mDamageData.setDamageTexture(damageTexture);
	}

	inline SimpleDamageData::EntityIdArrayInterface& SimpleDamageComponent::getDamageLevel1Particles()
	{
		return mDamageData.DamagedLevel1Particles;
	}

	inline SimpleDamageData::EntityIdArrayInterface& SimpleDamageComponent::getDamageLevel2Particles()
	{
		return mDamageData.DamagedLevel2Particles;
	}

	inline SimpleDamageData::EntityIdArrayInterface& SimpleDamageComponent::getDestroyedParticles()
	{
		return mDamageData.DestroyedParticles;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
