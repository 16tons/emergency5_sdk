// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ObjectDamageComponent::ObjectDamageComponent(qsf::Prototype* prototype) :
		DamageComponent(prototype)
	{
		// Nothing to do in here
	}

	inline ObjectDamageComponent::~ObjectDamageComponent()
	{
		// Nothing to do in here
	}

	inline bool ObjectDamageComponent::isReadyForCleanup() const
	{
		return DamageComponent::isReadyForCleanup();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
