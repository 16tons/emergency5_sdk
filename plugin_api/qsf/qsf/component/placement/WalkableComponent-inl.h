// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline WalkableComponent::WalkableComponent(Prototype* prototype) :
		Component(prototype),
		mLevel(0)
	{
		// Nothing to do in here
	}

	inline WalkableComponent::~WalkableComponent()
	{
		// Nothing to do in here
	}

	inline int32 WalkableComponent::getLevel() const
	{
		return mLevel;
	}

	inline const std::string& WalkableComponent::getTag() const
	{
		return mTag.getName();
	}

	inline uint32 WalkableComponent::getTagHash() const
	{
		return mTag.getHash();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
