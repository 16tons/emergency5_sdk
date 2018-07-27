// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline PersonSpawnPointComponent::PersonSpawnPointComponent(qsf::Prototype* prototype) :
		CivilistSpawnPointComponent(prototype),
		mSpawnPointType(PERSONSPAWNPOINTTYPE_GATE),
		PersonList(*this)
	{
		// Nothing to do in here
	}

	inline PersonSpawnPointComponent::~PersonSpawnPointComponent()
	{
		// Nothing to do in here
	}

	inline PersonSpawnPointComponent::PersonSpawnPointType PersonSpawnPointComponent::getPersonSpawnPointType() const
	{
		return mSpawnPointType;
	}

	inline void PersonSpawnPointComponent::setPersonSpawnPointType(PersonSpawnPointType personSpawnPointType)
	{
		mSpawnPointType = personSpawnPointType;
	}


	//[-------------------------------------------------------]
	//[ Public virtual em5::SpawnPointComponent methods       ]
	//[-------------------------------------------------------]
	inline bool PersonSpawnPointComponent::isEntry() const
	{
		return (mSpawnPointType == PERSONSPAWNPOINTTYPE_ENTRY || mSpawnPointType == PERSONSPAWNPOINTTYPE_GATE);
	}

	inline bool PersonSpawnPointComponent::isExit() const
	{
		return (mSpawnPointType == PERSONSPAWNPOINTTYPE_EXIT || mSpawnPointType == PERSONSPAWNPOINTTYPE_GATE);
	}

	inline bool PersonSpawnPointComponent::isTransit() const
	{
		return (mSpawnPointType == PERSONSPAWNPOINTTYPE_TRANSIT);
	}


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	inline std::size_t PersonSpawnPointComponent::PersonListInterface::size() const
	{
		return getOwner().mList.size();
	}

	inline const std::string& PersonSpawnPointComponent::PersonListInterface::get(std::size_t index) const
	{
		return getOwner().mList.at(index);
	}

	inline void PersonSpawnPointComponent::PersonListInterface::set(std::size_t index, const std::string& value)
	{
		getOwner().mList.at(index) = value;
	}

	inline void PersonSpawnPointComponent::PersonListInterface::insert(std::size_t before, const std::string& value)
	{
		getOwner().mList.insert(getOwner().mList.begin() + before, value);
	}

	inline void PersonSpawnPointComponent::PersonListInterface::remove(std::size_t index)
	{
		getOwner().mList.erase(getOwner().mList.begin() + index);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
