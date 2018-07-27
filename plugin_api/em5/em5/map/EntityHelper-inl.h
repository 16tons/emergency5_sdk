// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline EntityHelper::EntityHelper(qsf::Entity& entity) :
		qsf::EntityHelper(entity)
	{
		// Nothing here
	}

	inline EntityHelper::~EntityHelper()
	{
		// Nothing here
	}

	template<typename T>
	T* EntityHelper::getGameLogic() const
	{
		return static_cast<T*>(getGameLogicByTypeId(T::GAMELOGIC_TYPE_ID));
	}


	//[-------------------------------------------------------]
	//[ Misc                                                  ]
	//[-------------------------------------------------------]
	template<typename T>
	void EntityHelper::getComponentsFromChildren(std::vector<T*>& output) const
	{
		getComponentsFromChildrenById(T::COMPONENT_ID, reinterpret_cast<std::vector<qsf::Component*>&>(output));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
