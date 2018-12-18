// Copyright (C) 2012-2018 Promotion Software GmbH


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
	inline ContaminationComponent::ContaminationComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype),
		mContaminationType(Injury::CONTAMINATION_NONE),
		mContaminationLimit(8),
		mContaminatedPersons(0),
		CloudArray(*this, mContaminationCloudIds)
	{
		// Nothing to do in here
	}

	inline ContaminationComponent::~ContaminationComponent()
	{
		// Nothing to do in here
	}

	inline Injury::ContaminationType ContaminationComponent::getContaminationType() const
	{
		return mContaminationType;
	}

	inline void ContaminationComponent::setContaminationType(ContaminationType contaminationType)
	{
		mContaminationType = contaminationType;
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t ContaminationComponent::EntityIdArrayInterface::size() const
	{
		return getVariable().size();
	}

	inline const uint64& ContaminationComponent::EntityIdArrayInterface::get(std::size_t index) const
	{
		return getVariable()[index];
	}

	inline void ContaminationComponent::EntityIdArrayInterface::set(std::size_t index, const uint64& value)
	{
		getVariable()[index] = value;
	}

	inline void ContaminationComponent::EntityIdArrayInterface::insert(std::size_t before, const uint64& value)
	{
		getVariable().insert(getVariable().begin() + before, value);
	}

	inline void ContaminationComponent::EntityIdArrayInterface::remove(std::size_t index)
	{
		getVariable().erase(getVariable().begin() + index);
	}

	inline uint32 ContaminationComponent::getContaminationLimit() const
	{
		return mContaminationLimit;
	}

	inline void ContaminationComponent::setContaminationLimit(uint32 contaminationLimit)
	{
		mContaminationLimit = contaminationLimit;
	}

	inline uint32 ContaminationComponent::getContaminatedPersons() const
	{
		return mContaminatedPersons;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
