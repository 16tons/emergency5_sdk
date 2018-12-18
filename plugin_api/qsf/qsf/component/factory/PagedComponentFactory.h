// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/component/factory/ComponentFactory.h"
#include "qsf/base/manager/PagedAllocator.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Paged component collection class template
	*/
	template<typename COMPONENT>
	class PagedComponentFactory : public ComponentFactory
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param pageSize
		*    Number of components to be stored in the same chunk of memory
		*
		*  @note
		*    - Unlike the ComponentFactory constructor, there's no need to pass the component ID for it is already defined by the template parameter
		*/
		PagedComponentFactory(size_t pageSize);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PagedComponentFactory();

		/**
		*  @brief
		*    Static factory method
		*
		*  @param pageSize
		*    Number of components to be stored in the same chunk of memory
		*
		*  @note
		*    - This is not necessary, but somehow looks nicer than the constructor call when used
		*/
		static ComponentFactory& createInstance(size_t pageSize);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ComponentFactory methods          ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Create a component instance
		*
		*  @return
		*    Create a component instance, null pointer on error, you're responsible for destroying the instance in case you no longer need it
		*/
		virtual Component* createComponent(Prototype& prototype) override;

		/**
		*  @brief
		*    Destroy a component instance
		*
		*  @param[in] component
		*    Component instance to destroy, after this method call this instance is considered to be no longer valid
		*/
		virtual void destroyComponent(Component& component) override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		PagedAllocator<COMPONENT>* mAllocator;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/factory/PagedComponentFactory-inl.h"
