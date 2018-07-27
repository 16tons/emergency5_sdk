// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LinkComponentHelper::~LinkComponentHelper()
	{
		// Nothing to do in here
	}

	template<typename T>
	void LinkComponentHelper::getAllComponentsFromPrototypeOrLinkedChild(std::vector<T*>& result) const
	{
		getAllComponentsFromPrototypeOrLinkedChildById(T::COMPONENT_ID, reinterpret_cast<std::vector<Component*>&>(result));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
