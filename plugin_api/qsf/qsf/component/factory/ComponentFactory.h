// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
	class Prototype;
	class ClassSystem;
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
	*    Component factory base class
	*/
	class QSF_API_EXPORT ComponentFactory : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ComponentSystem;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the ID of the component type this component manager is responsible for
		*
		*  @return
		*    The ID of the component type this component manager is responsible for
		*/
		inline uint32 getComponentId() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::ComponentFactory methods          ]
	//[-------------------------------------------------------]
	public:
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
		inline virtual Component* createComponent(Prototype& prototype);

		/**
		*  @brief
		*    Destroy a component instance
		*
		*  @param[in] component
		*    Component instance to destroy, after this method call this instance is considered to be no longer valid
		*/
		inline virtual void destroyComponent(Component& component);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] componentId
		*    ID of the component type this component manager is responsible for
		*/
		ComponentFactory(uint32 componentId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComponentFactory();

		/**
		*  @brief
		*    Create a component instance using CAMP as a factory
		*/
		Component* createComponentUsingCamp(Prototype& prototype);

		/**
		*  @brief
		*    Clone properties from originalComponent to component
		*/
		void clonePropertiesUsingCamp(Component& component, const Component& originalComponent);

		/**
		*  @brief
		*    Destroy a component instance
		*/
		void destroyUsingGenericFallback(Component& component);

		/**
		*  @brief
		*    Called internally after creation of a component
		*/
		inline Component* incExisting(Component* component);

		/**
		*  @brief
		*    Called internally before deleting a component
		*/
		inline void decExisting();


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		uint32 mNumExistingComponents;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32			   mComponentId;	///< ID of the component type this component factory can create and destroy
		const camp::Class* mCampClass;		///< CAMP class instance for this component factory; may be a null pointer in case the component ID is not valid


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/factory/ComponentFactory-inl.h"
