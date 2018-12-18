// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	template<typename COMPONENT>
	PagedComponentFactory<COMPONENT>::PagedComponentFactory(size_t pageSize) :
		ComponentFactory(COMPONENT::COMPONENT_ID),
		mAllocator(new PagedAllocator<COMPONENT>(pageSize))
	{
		// Nothing here
	}

	template<typename COMPONENT>
	PagedComponentFactory<COMPONENT>::~PagedComponentFactory()
	{
		// The factory must not be destroyed while there are still managed components
		QSF_ASSERT(mAllocator->empty(), "There are still components using this factory that will get invalidated because their memory is freed!", QSF_REACT_NONE);

		// Delete the allocator
		delete mAllocator;
	}

	template<typename COMPONENT>
	ComponentFactory& PagedComponentFactory<COMPONENT>::createInstance(size_t pageSize)
	{
		return *(new PagedComponentFactory<COMPONENT>(pageSize));
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ComponentFactory methods          ]
	//[-------------------------------------------------------]
	template<typename COMPONENT>
	Component* PagedComponentFactory<COMPONENT>::createComponent(Prototype& prototype)
	{
		// Let the allocator create a new component and pass the prototype to the constructor
		return incExisting(mAllocator->create(&prototype));
	}

	template<typename COMPONENT>
	void PagedComponentFactory<COMPONENT>::destroyComponent(Component& component)
	{
		// Let the allocator destroy the component
		mAllocator->destroy(static_cast<COMPONENT*>(&component));
		decExisting();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
