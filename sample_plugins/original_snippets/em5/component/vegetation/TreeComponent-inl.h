// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline TreeComponent::TreeComponent(qsf::Prototype* prototype) :
		DamageComponent(prototype),
		mTreeParticlesEnabled(true),
		mTreeFallDownAnimation(nullptr)
	{
		// Nothing to do in here
	}

	inline TreeComponent::~TreeComponent()
	{
		// Nothing to do in here
	}

	inline bool TreeComponent::isReadyForCleanup() const
	{
		return DamageComponent::isReadyForCleanup() || mInternalFlags.isSet(TREE_IS_CUT);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
