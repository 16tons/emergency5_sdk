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

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
	class Prototype;
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
	*    Component collection base class
	*/
	class QSF_API_EXPORT ComponentCollection : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class ComponentManager;	// Component manager needs to access private constructor and destructor


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef std::vector<Component*> ComponentArray;

		struct BaseChildPair
		{
			ComponentCollection& mBase;
			ComponentCollection& mChild;
			uint32				 mIndex;
			BaseChildPair(ComponentCollection& base, ComponentCollection& child, uint32 index) : mBase(base), mChild(child), mIndex(index) {}
		};

		// Iterator for cycling through all components of this collection and its child collections
		struct iterator
		{
		public:
			typedef size_t difference_type;
			typedef Component* value_type;
			typedef Component*& reference;
			typedef Component* pointer;

			inline iterator() : mPair(nullptr), mIndex(0) {}
			inline iterator(const ComponentCollection& collection);

			inline bool operator==(const iterator& other) const;
			inline bool operator!=(const iterator& other) const;
			inline iterator& operator++();
			inline iterator& operator++(int);
			inline Component* operator*() const;

		protected:
			const BaseChildPair* mPair;
			size_t				 mIndex;
		};

		template<typename T>
		class ComponentList
		{
		public:
			struct iterator : ComponentCollection::iterator
			{
			public:
				inline iterator() : ComponentCollection::iterator() {}
				inline iterator(const ComponentCollection& collection) : ComponentCollection::iterator(collection) {}

				inline T* operator*() const  { return static_cast<T*>(ComponentCollection::iterator::operator*()); }
			};

		public:
			inline ComponentList(const ComponentCollection& collection) : mCollection(collection) {}

			inline bool empty() const;
			inline size_t size() const;

			inline iterator begin() const;
			inline iterator end() const;

			inline std::vector<T*> copyToVector() const;	// C++11 compilers will optimize this, so no actual copying is performed
			inline void copyToVector(std::vector<T*>& outVector, bool clearOutput = false) const;

		private:
			const ComponentCollection& mCollection;
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the ID of the component type this component collection is responsible for
		*
		*  @return
		*    The ID of the component type this component collection is responsible for
		*/
		inline uint32 getComponentId() const;

		/**
		*  @brief
		*    Return listing of components
		*
		*  @return
		*    An iterable list of component pointers that are registered at this component collection and its children
		*/
		inline const ComponentList<Component>& getComponents() const;

		/**
		*  @brief
		*    Return listing of components
		*
		*  @return
		*    An iterable list of component pointers that are registered at this component collection and its children
		*/
		template<typename T>
		inline const ComponentList<T>& getComponentList() const;

		/**
		*  @brief
		*    Reserve space to hold the given number of components
		*
		*  @param[in] count
		*    Total number of component to reserve space for
		*/
		void reserveComponents(uint32 count);

		inline bool empty() const;
		inline size_t size() const;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] componentId
		*    ID of the component type this component collection is responsible for
		*  @param[in] baseCollection
		*    Pointer to the base component collection
		*/
		ComponentCollection(uint32 componentId, ComponentCollection* baseCollection);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ComponentCollection();

		/**
		*  @brief
		*    Registers a component; don't call this for a component that is already registered
		*/
		void registerComponent(Component& component);

		/**
		*  @brief
		*    Unregisters a component; throws an exception if it is not registered
		*/
		void unregisterComponent(Component& component);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void registerChildRecursively(ComponentCollection& child);
		void changeSizeRecursively(int change);

		const BaseChildPair* getFirstBaseChildPair() const;
		const BaseChildPair* getNextBaseChildPair(const BaseChildPair& pair) const;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<BaseChildPair> BaseChildPairArray;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32				 mComponentId;			///< ID of the component type this component manager is responsible for
		ComponentArray		 mComponentArray;		///< Stores pointers to all currently existing components of exactly this component type
		size_t				 mSize;					///< Total number of components of this collection's type and all child types

		ComponentCollection* mBaseCollection;		///< Pointer to the base component collection holding all instances of the component's base class; may be a null pointer
		BaseChildPairArray	 mChildCollections;		///< Pointers to the direct and indirect child component collections; first entry refers to this collection, not a child

		ComponentList<Component> mComponentList;	///< Component list instance we can return as reference; it has no other use


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/component/ComponentCollection-inl.h"
