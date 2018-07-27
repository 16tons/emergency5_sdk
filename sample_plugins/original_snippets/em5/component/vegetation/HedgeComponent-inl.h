// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline HedgeComponent::HedgeComponent(qsf::Prototype* prototype) :
		DamageComponent(prototype),
		mDestroyedGroundDecalMesh(),
		mGroundDecalEntityId(qsf::getUninitialized<uint64>())
	{
		// Nothing to do in here
	}

	inline HedgeComponent::~HedgeComponent()
	{
		// Nothing to do in here
	}

	inline bool HedgeComponent::isReadyForCleanup() const
	{
		return DamageComponent::isReadyForCleanup() || mInternalFlags.isSet(HEDGE_IS_CUT);
	}

	inline const qsf::AssetProxy& HedgeComponent::getDestroyedGroundDecalMesh() const
	{
		return mDestroyedGroundDecalMesh;
	}

	inline void HedgeComponent::setDestroyedGroundDecalMesh(const qsf::AssetProxy& destroyedGroundDecalMesh)
	{
		mDestroyedGroundDecalMesh = destroyedGroundDecalMesh;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
