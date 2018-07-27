// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LookAtComponent::LookAtComponent(Prototype* prototype) :
		Component(prototype),
		mTargetEntityId(getUninitialized<uint64>())
	{
		// Nothing to do in here
	}

	inline LookAtComponent::~LookAtComponent()
	{
		// Nothing to do in here
	}

	inline uint64 LookAtComponent::getTargetEntityId() const
	{
		return mTargetEntityId;
	}

	inline void LookAtComponent::setTargetEntityId(const uint64 targetEntityId)
	{
		assignAndPromoteChange(mTargetEntityId, targetEntityId, TARGET_ENTITY_ID);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
