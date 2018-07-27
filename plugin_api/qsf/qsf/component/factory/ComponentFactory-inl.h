// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline uint32 ComponentFactory::getComponentId() const
	{
		return mComponentId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ComponentFactory methods          ]
	//[-------------------------------------------------------]
	inline Component* ComponentFactory::createComponent(Prototype& prototype)
	{
		// Use CAMP as a factory
		return incExisting(createComponentUsingCamp(prototype));
	}

	inline void ComponentFactory::destroyComponent(Component& component)
	{
		// Use generic fallback
		destroyUsingGenericFallback(component);
		decExisting();
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline Component* ComponentFactory::incExisting(Component* component)
	{
		if (nullptr != component)
			++mNumExistingComponents;
		return component;
	}

	inline void ComponentFactory::decExisting()
	{
		--mNumExistingComponents;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
