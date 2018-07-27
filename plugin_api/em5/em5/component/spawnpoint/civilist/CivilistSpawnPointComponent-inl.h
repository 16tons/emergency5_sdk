// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline CivilistSpawnPointComponent::~CivilistSpawnPointComponent()
	{
		// Nothing to do in here
	}

	inline uint32 CivilistSpawnPointComponent::getSpawnGroupId() const
	{
		return mSpawnGroup.getHash();
	}

	inline const std::string& CivilistSpawnPointComponent::getSpawnGroupName() const
	{
		return mSpawnGroup.getName();
	}

	inline uint32 CivilistSpawnPointComponent::getTagHash() const
	{
		return mTag.getHash();
	}

	inline const std::string& CivilistSpawnPointComponent::getTag() const
	{
		return mTag.getName();
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline CivilistSpawnPointComponent::CivilistSpawnPointComponent(qsf::Prototype* prototype) :
		SpawnPointComponent(prototype)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
