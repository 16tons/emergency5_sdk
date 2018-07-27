// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/base/UniqueInstance.h"
#include "qsf/base/manager/PagedAllocator.h"
#ifdef QSF_PROFILING
	#include "qsf/time/profiling/ProfilingElement.h"
#endif

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Prototype;
	class Component;
	class ClassSystem;
	class ComponentFactory;
}
namespace camp
{
	class Class;
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
	*    The component system manages component factories, but does not contain a component manager
	*    for we need one of these in every map and in the prototype system as well.
	*/
	class QSF_API_EXPORT ComponentSystem : public System, public qsf::UniqueInstance<ComponentSystem>
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ComponentManager;	// Component manager calls create/clone/destroy methods


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum ClassRelation
		{
			CLASS_IS_IDENTICAL,		///< The two component classes compared are actually the same class
			CLASS_IS_BASE,			///< The first component class is base (or ancestor over multiple levels) of the second one
			CLASS_IS_DERIVED,		///< The second component class is base (or ancestor over multiple levels) of the first one
			CLASS_SHARES_ANCESTOR,	///< Both component classes share a common ancestor, but none of both is ancestor of the others (e.g. one is a sibling of the other)
			CLASS_NOT_RELATED,		///< There is no common ancestor class of the two component classes
			CLASS_COMPARISON_ERROR	///< Error during comparison, e.g. one of the classes is unknown
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ComponentSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComponentSystem();

		/**
		*  @brief
		*    Check relationship between two component classes
		*
		*  @param[in] firstComponentId
		*    ID of the first component class; this is the string hash of the class name as registered in CAMP
		*  @param[in] secondComponentId
		*    ID of the second component class; this is the string hash of the class name as registered in CAMP
		*
		*  @return
		*    The relation between the two classes
		*/
		ClassRelation compareComponentClassesById(uint32 firstComponentId, uint32 secondComponentId) const;

		/**
		*  @brief
		*    Get a component class's topmost ancestor component class IDs
		*
		*  @param[in] componentId
		*    ID of the component class
		*
		*  @return
		*    ID of the topmost ancestor component class; this can be the passed component ID itself
		*
		*  @remarks
		*    The topmost ancestor is always a component class below either "qsf::Component" or a root class
		*    as defined as CAMP class "RootClass"
		*
		*  @throw
		*    - Throws an exception if the passed component ID is unknown
		*/
		inline uint32 getComponentClassTopmostAncestorId(uint32 componentId) const;

		/**
		*  @brief
		*    Get a vector of all ancestor component class IDs for a given component ID
		*
		*  @param[in] componentId
		*    ID of the component class to get ancestors of
		*
		*  @return
		*    A vector containing component IDs of base classes, beginning with the direct base component; can be empty
		*
		*  @remarks
		*    The returned list does only contain ancestor classes below (but not including) "qsf::Component" or a root class
		*    as defined as CAMP class "RootClass"
		*
		*  @throw
		*    - Throws an exception if the passed component ID is unknown
		*/
		inline const std::vector<uint32>& getComponentClassAncestorIds(uint32 componentId) const;

		/**
		*  @brief
		*    Register a user-created component factory
		*
		*  @param[in] factory
		*    The factory to register; the component system will take over the ownership of the factory and care for its destruction
		*
		*  @remarks
		*    There can only be one component factory for each component type (= component ID).
		*    If there is already a factory for the same ID registered, this one will be destroyed and replaced.
		*    Destruction of factories that already created components (and may manage their memory) can be fatal for some
		*    kinds of component factory classes (e.g. the PagedComponentFactory, it will throw an exception is this case).
		*    Anyway, this is no problem for the default component factory implementation that is automatically used
		*    if there is no factory registered explicitly.
		*/
		void registerComponentFactory(ComponentFactory& factory);

		#ifdef QSF_PROFILING
			ProfilingElement& getProfilingElementByComponentId(uint32 id)
			{
				return mProfilingElementMap[id];
			}

			void logProfilingResults();
		#endif


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
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


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called after a new CAMP class has been registered
		*
		*  @param[in] added
		*    New CAMP class
		*/
		void classAdded(const camp::Class& added);

		ComponentFactory& getComponentFactoryById(uint32 componentId);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct ComponentClassInfo
		{
			uint32 mComponentId;			///< The component ID
			const camp::Class* mCampClass;	///< Pointer to CAMP class; do not destroy the instance
			ComponentClassInfo* mBase;		///< Base component class info; may be a null pointer if there is no base class (note that "qsf::Component" itself and some others do not count)
			uint32 mTopmostAncestorId;		///< Component ID of topmost ancestor in derivation hierarchy; may be the component ID itself
			std::vector<uint32> mAncestorIds;
		};
		typedef boost::container::flat_map<uint32, ComponentClassInfo*> ComponentClassInfoMap;
		typedef PagedAllocator<ComponentClassInfo> ComponentClassInfoAllocator;

		typedef boost::container::flat_map<uint32, ComponentFactory*> ComponentFactoryMap;

		#ifdef QSF_PROFILING
			typedef boost::container::flat_map<uint32, ProfilingElement> ProfilingElementMap;
		#endif


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ClassSystem&				mClassSystem;
		ComponentClassInfoMap		mComponentClassInfoMap;
		ComponentClassInfoAllocator mComponentClassInfoAllocator;
		ComponentFactoryMap			mComponentFactoryMap;			///< Map of component factory instances, unique component ID as key and component factory instance as value, do not destroy the instances

		#ifdef QSF_PROFILING
			ProfilingElementMap mProfilingElementMap;
		#endif


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/ComponentSystem-inl.h"
