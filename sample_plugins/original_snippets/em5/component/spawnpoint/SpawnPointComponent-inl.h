// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline SpawnPointComponent::SpawnPointComponent(qsf::Prototype* prototype) :
		qsf::Component(prototype)
	{
		// Nothing to do in here
	}

	inline SpawnPointComponent::~SpawnPointComponent()
	{
		// Nothing to do in here
	}

	inline const std::vector<std::string>& SpawnPointComponent::getList() const
	{
		return mList;
	}

	inline void SpawnPointComponent::setList(const std::vector<std::string>& list)
	{
		mList = list;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
