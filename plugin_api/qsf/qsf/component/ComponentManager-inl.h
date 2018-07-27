// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/error/ErrorHandling.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline ComponentManager::~ComponentManager()
	{
		QSF_CHECK(mComponentCollectionMap.empty(), "Component collection map has to be emptied before destroying the component manager!", QSF_REACT_NONE);
	}

	template <typename T>
	const ComponentCollection::ComponentList<T>& ComponentManager::getComponentInstances()
	{
		const ComponentCollection& collection = getComponentCollectionById(T::COMPONENT_ID);
		return collection.getComponentList<T>();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
