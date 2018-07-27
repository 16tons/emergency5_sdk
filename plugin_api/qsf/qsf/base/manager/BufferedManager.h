// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/manager/Manager.h"

#include <vector>
#include <boost/container/flat_map.hpp>


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
	*    Buffered manager class template
	*
	*  @remarks
	*    This template implements a manager of elements mapped by a key (e.g. an integer ID or similar).
	*    Write access to the map of elements can be blocked, the changes are applied at a safe point of time then.
	*/
	template <typename KEY, typename ELEMENT, typename CONTAINER = boost::container::flat_map<KEY, ELEMENT>>
	class BufferedManager : public Manager
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef KEY     ElementKey;
		typedef ELEMENT Element;

		typedef CONTAINER ElementMap;


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		BufferedManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~BufferedManager();

		/**
		*  @brief
		*    Return map of elements
		*/
		const ElementMap& getElements() const;

		/**
		*  @brief
		*    Find a specific element by its key in either the element map or the insertion buffer
		*/
		Element* findElement(const ElementKey& key);
		const Element* findElement(const ElementKey& key) const;

		/**
		*  @brief
		*    Insert a new element
		*/
		bool insertElement(const ElementKey& key, Element& element);

		/**
		*  @brief
		*    Remove an element by its key
		*/
		bool removeElement(const ElementKey& key);

		/**
		*  @brief
		*    Remove all elements
		*/
		void removeAllElements();

		/**
		*  @brief
		*    Return write protection flag
		*/
		bool getWriteProtection() const;

		/**
		*  @brief
		*    Enable or disable write protection
		*/
		void setWriteProtection(bool enable);

		/**
		*  @brief
		*    Insert new elements that have been added while the write protection was active
		*/
		void flushInsertionBuffer();

		/**
		*  @brief
		*    Remove all elements that have been marked for removal while the write protection was active
		*/
		void flushRemovalBuffer();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::BufferedManager methods        ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Called whenever an element is inserted into the element map
		*/
		virtual void onElementInsertion(const ElementKey& key, Element& element);

		/**
		*  @brief
		*    Called whenever an element is actually removed
		*/
		virtual void onElementRemoval(const ElementKey& key, Element& element);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef typename std::vector<ElementKey> ElementKeyArray;

		ElementMap mElements;				///< Currently active elements, mapped by their keys
		ElementMap mInsertionBuffer;		///< Lists all elements to be inserted
		ElementKeyArray mRemovalBuffer;		///< Lists all elements that are marked to be removed

		int32 mWriteProtectionCount;		///< If ">0", mElements may not directly be changed, but changes are written to the buffers instead


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/manager/BufferedManager-inl.h"
