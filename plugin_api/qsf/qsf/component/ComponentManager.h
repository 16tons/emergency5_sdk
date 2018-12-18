// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/component/ComponentCollection.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace camp
{
	class Class;
}
namespace qsf
{
	class Prototype;
	class Component;
	class ClassSystem;
	class ComponentSystem;
	class ComponentCollection;
}


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
	*    Component system class
	*
	*  @remarks
	*    Optionally the memory management of certain component types can be out-sourced. In case
	*    a component type has no component collection assigned, a default implementation is used.
	*/
	class QSF_API_EXPORT ComponentManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class Prototype;
		friend class ComponentCollection;


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_map<uint32, ComponentCollection*> ComponentCollectionMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ComponentManager();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~ComponentManager();

		/**
		*  @brief
		*    Create a component instance
		*
		*  @param[in] componentId
		*    ID of the component type to instantiate
		*  @param[in] prototype
		*    Prototype to instantiate the component in
		*
		*  @return
		*    Create a component instance, null pointer on error, you're responsible for destroying the instance in case you no longer need it
		*/
		Component* createComponent(uint32 componentId, Prototype& prototype);

		/**
		*  @brief
		*    Destroy a component instance
		*
		*  @param[in] component
		*    Component instance to destroy, after this method call this instance is considered to be no longer valid
		*/
		void destroyComponent(Component& component);

		/**
		*  @brief
		*    Register a component at all collections that are affected
		*
		*  @param[in] component
		*    Component instance to register
		*/
		void registerComponentAtCollections(Component& component);

		/**
		*  @brief
		*    Unregister a component at all collections that are affected
		*
		*  @param[in] component
		*    Component instance to unregister
		*/
		void unregisterComponentAtCollections(Component& component);

		/**
		*  @brief
		*    Destroy all existing component collections
		*/
		void destroyAllComponentCollections();

		/**
		*  @brief
		*    Return the component collection that holds all instances of the given component ID
		*/
		ComponentCollection& getComponentCollectionById(uint32 componentId);
		ComponentCollection* tryGetComponentCollectionById(uint32 componentId);

		inline const ComponentCollectionMap& getComponentCollectionMap() const  { return mComponentCollectionMap; }

		/**
		*  @brief
		*    Helper function to get all components of the given type
		*/
		template <typename T>
		const ComponentCollection::ComponentList<T>& getComponentInstances();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		uint32 getBaseComponentId(const camp::Class& campClass) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ClassSystem&			mClassSystem;
		ComponentSystem&		mComponentSystem;
		ComponentCollectionMap	mComponentCollectionMap;	///< Map of component collection instances, unique component ID as key and component collection instance as value, do not destroy the instances


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/ComponentManager-inl.h"
