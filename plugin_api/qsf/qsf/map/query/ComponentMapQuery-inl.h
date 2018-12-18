// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ComponentMapQuery::ComponentMapQuery(const Map& map) :
		MapQuery(map)
	{
		// Nothing to do in here
	}

	inline ComponentMapQuery::~ComponentMapQuery()
	{
		// Nothing to do in here
	}

	template <typename T>
	T* ComponentMapQuery::getFirstInstance() const
	{
		return static_cast<T*>(getFirstInstanceById(T::COMPONENT_ID));
	}

	template <typename T>
	const ComponentCollection::ComponentList<T>& ComponentMapQuery::getAllInstances() const
	{
		return reinterpret_cast<const ComponentCollection::ComponentList<T>&>(getAllInstancesById(T::COMPONENT_ID));
	}

	template <typename T>
	bool ComponentMapQuery::getInstancesInCircle(const glm::vec3& center, float radius, std::vector<T*>& outComponents) const
	{
		return getInstancesInCircleById(T::COMPONENT_ID, center, radius, reinterpret_cast<std::vector<Component*>&>(outComponents));
	}

	template <typename T>
	bool ComponentMapQuery::getInstancesInBox(const Transform& boxTransform, bool yAxisCentered, std::vector<T*>& outComponents) const
	{
		return getInstancesInBoxById(T::COMPONENT_ID, boxTransform, yAxisCentered, reinterpret_cast<std::vector<Component*>&>(outComponents));
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
