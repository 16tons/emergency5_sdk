// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SupportLegsComponent::SupportLegsComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mSupportLegsState(SUPPORTLEGSSTATE_HIDDEN),
		SupportLegsArray(*this, mSupportLegIds)
	{
		// Nothing to do in here
	}

	inline SupportLegsComponent::~SupportLegsComponent()
	{
		// Nothing to do in here
	}

	inline const qsf::AssetProxy& SupportLegsComponent::getAnimationExtend() const
	{
		return mAnimationExtend;
	}

	inline void SupportLegsComponent::setAnimationExtend(const qsf::AssetProxy& skeleton)
	{
		mAnimationExtend = skeleton;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t SupportLegsComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& SupportLegsComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void SupportLegsComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void SupportLegsComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void SupportLegsComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
