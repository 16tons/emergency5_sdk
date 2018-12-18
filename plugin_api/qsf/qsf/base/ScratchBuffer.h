// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>

#include <vector>
#include <list>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
	class LinkComponent;
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
	*    Scratch buffer base template class
	*/
	template <typename T>
	class ScratchBuffer : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		typedef T Element;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline ScratchBuffer();

		/**
		*  @brief
		*    Constructor with a number of elements to reserve already
		*/
		inline explicit ScratchBuffer(size_t reserveSize);

		/**
		*  @brief
		*    Destructor
		*/
		inline ~ScratchBuffer();

		//[-------------------------------------------------------]
		//[ Operators                                             ]
		//[-------------------------------------------------------]
		inline operator Element&();
		inline Element& operator*();
		inline Element* operator->();


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		struct Instance
		{
			bool mInUse;
			Element mElement;
		};


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		Instance& mInstance;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		// Use an std::list here to have stable instances and get element destruction for free; that would not be the case with "std::vector<Instance*>"
		typedef std::list<Instance> InstanceBuffer;


	//[-------------------------------------------------------]
	//[ Private static methods                                ]
	//[-------------------------------------------------------]
	private:
		static Instance& getInstance();
		static void freeInstance(Instance& instance);


	//[-------------------------------------------------------]
	//[ Private static data                                   ]
	//[-------------------------------------------------------]
	private:
		static InstanceBuffer mInstanceBuffer;


	};


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Scratch buffer specialized template class for std::vectors
	*/
	template <typename T>
	class StdVectorScratchBuffer : public ScratchBuffer<std::vector<T>>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		inline StdVectorScratchBuffer() : ScratchBuffer<std::vector<T>>() {}
		inline explicit StdVectorScratchBuffer(size_t reserveSize) : ScratchBuffer<std::vector<T>>(reserveSize) {}
		inline explicit StdVectorScratchBuffer(const std::vector<T>& toCopy) : ScratchBuffer<std::vector<T>>(toCopy.size()) { this->mInstance.mElement = toCopy; }

		inline void replaceWithBack(size_t index);

		//[-------------------------------------------------------]
		//[ Shortcuts for std::vector access                      ]
		//[-------------------------------------------------------]
		inline typename ScratchBuffer<std::vector<T>>::Element::iterator begin();
		inline typename ScratchBuffer<std::vector<T>>::Element::iterator end();
		inline bool empty() const;
		inline size_t size() const;
		inline void reserve(size_t size);
		inline void push_back(const T& element);

		template<class... argTs>
		inline void emplace_back(argTs&&... args);
		inline StdVectorScratchBuffer<T>& operator=(const std::vector<T>& toCopy);
		inline T& operator[](size_t index);
		inline const T& operator[](size_t index) const;


	};


	// Some concrete typedefs
	typedef ScratchBuffer<std::string>			   StdStringScratchBuffer;
	typedef StdVectorScratchBuffer<Entity*>		   EntityVectorScratchBuffer;
	typedef StdVectorScratchBuffer<LinkComponent*> LinkComponentVectorScratchBuffer;


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/base/ScratchBuffer-inl.h"
