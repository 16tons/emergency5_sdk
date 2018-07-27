// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"
#include "qsf/base/idgenerator/ConsecutiveIdGenerator.h"

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Helper definitions                                    ]
	//[-------------------------------------------------------]
	namespace detail
	{
		template<typename T>
		class DirectAllocator
		{
		public:
			T* create()
			{
				return new T();
			}

			template<typename A>
			T* create(A a)
			{
				return new T(a);
			}

			void destroy(T* element)
			{
				delete element;
			}

			void clear()
			{
				// Default implementation is empty
			}
		};
	}


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Element manager class template
	*
	*  @remarks
	*    This template implements a manager of elements mapped by a key (e.g. an integer ID or similar).
	*    The element manager is responsible for the creation and destruction of the elements managed by it.
	*/
	template <typename KEY, typename ELEMENT, typename ALLOCATOR = detail::DirectAllocator<ELEMENT>, typename ID_GENERATOR = ConsecutiveIdGenerator<KEY>>
	class ElementManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef boost::container::flat_map<KEY, ELEMENT*> ElementMap;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		ElementManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~ElementManager();

		/**
		*  @brief
		*    Return map of elements
		*
		*  @return
		*    Map of elements, do not destroy the instance
		*/
		const ElementMap& getElements() const;

		/**
		*  @brief
		*    Find a specific element by its key in either the element map
		*
		*  @return
		*    Specific element, null pointer on error, do not destroy the instance
		*/
		ELEMENT* findElement(const KEY& key) const;

		/**
		*  @brief
		*    Reserve elements
		*
		*  @param[in] numberOfElements
		*    Number of elements to reserve
		*
		*  @note
		*    - You might want to reserve elements before starting an insertion of element masses (performance)
		*/
		void reserveElements(size_t numberOfElements);

		/**
		*  @brief
		*    Create a new element
		*
		*  @return
		*    The created element instance, null pointer on error, do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique element ID instead)
		*/
		ELEMENT* createElement();

		/**
		*  @brief
		*    Create a new element with an enforced given unique element identifier
		*
		*  @param[in] key
		*    Key of the element to insert
		*
		*  @return
		*    The created element instance, null pointer on error, do no destroy the returned
		*    instance and do need keep a reference to it outside the C runtime stack (use the unique element ID instead)
		*
		*  @note
		*    - Use "createElement()" instead of "createElementById()" whenever possible, do not enforce unique element identifier if you can avoid it
		*/
		ELEMENT* createElementById(const KEY& key);

		/**
		*  @brief
		*    Insert an element with an enforced given unique element identifier
		*
		*  @param[in] key
		*    Key of the element to insert
		*  @param[in] element
		*    The element to insert; ownership is transferred to the element manager, so do no destroy the instance and do need keep a reference to it outside the C runtime stack (use the unique element ID instead)
		*
		*  @return
		*    "true" if insertion succeeds, "false" if the given key is already in use
		*
		*  @note
		*    - Use "createElement()" instead of "insertElement()" whenever possible, do not enforce unique element identifier if you can avoid it
		*    - Use this method in case you have to use your own construction process for elements, e.g. if you need to create sub-class instances of the ELEMENT class
		*    - You have to take care of assigning the given key to the element yourself
		*/
		bool insertElement(const KEY& key, ELEMENT& element);

		/**
		*  @brief
		*    Destroy an element by its key
		*
		*  @param[in] key
		*    Key of the element to destroy
		*
		*  @return
		*    "true" if all went fine, else "false"
		*/
		bool destroyElement(const KEY& key);

		/**
		*  @brief
		*    Destroy an element by iterator
		*
		*  @param[in] iterator
		*    Iterator pointing to the element to destroy
		*
		*  @return
		*    Updated iterator
		*/
		typename ElementManager<KEY, ELEMENT, ALLOCATOR, ID_GENERATOR>::ElementMap::iterator destroyElement(typename ElementMap::iterator iterator);

		/**
		*  @brief
		*    Destroy all elements
		*/
		void destroyAllElements();

		/**
		*  @brief
		*    Remove an element from the element manager without destroying it
		*
		*  @param[in] key
		*    Key of the element to insert
		*
		*  @return
		*    The element to grab, or null pointer if none was found under this key; you are responsible for destroying the instance
		*/
		ELEMENT* grabElement(const KEY& key);

		/**
		*  @brief
		*    Generate a new, unused key
		*
		*  @return
		*    The new key
		*/
		KEY generateId();


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Return map of elements
		*
		*  @return
		*    Map of elements, do not destroy the instance
		*/
		ElementMap& getElementsWritable();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::ElementManager methods         ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called whenever an element is inserted into the element map
		*
		*  @param[in] key
		*    Key of the inserted element
		*  @param[in] element
		*    Inserted element
		*
		*  @note
		*    - The default implementation is empty
		*/
		virtual void onElementInsertion(const KEY& key, ELEMENT& element);

		/**
		*  @brief
		*    Called whenever an element is actually going to be destroyed
		*
		*  @param[in] key
		*    Key of the destroyed element
		*  @param[in] element
		*    Destroyed element
		*
		*  @note
		*    - The default implementation is empty
		*/
		virtual void onElementRemoval(const KEY& key, ELEMENT& element);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		ID_GENERATOR mIdGenerator;	///< Element ID generator
		ALLOCATOR	 mAllocator;	///< Element allocator
		ElementMap	 mElements;		///< Elements, mapped by their keys


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/manager/ElementManager-inl.h"
