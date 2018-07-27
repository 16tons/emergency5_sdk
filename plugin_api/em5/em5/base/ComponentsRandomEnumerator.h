// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/base/RandomEnumerator.h"

#include <qsf/component/Component.h>
#include <qsf/map/query/ComponentMapQuery.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Helper class for iterating over a list of component instances of a specific type, in a random order
	*
	*  @remarks
	*    No component will be returned twice.
	*    Can be used to iterate either over all instances in the main map, or a custom list of components.
	*/
	template<typename T>
	class ComponentsRandomEnumerator : public RandomEnumerator<T>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor that will use all of main map's components to chose from
		*/
		inline explicit ComponentsRandomEnumerator(uint32 maxTries = std::numeric_limits<uint32>::max()) :
			RandomEnumerator<T>(maxTries)
		{
			// Fill our component list with all component instances in the main map
			const auto& components = qsf::ComponentMapQuery(QSF_MAINMAP).getAllInstances<T>();
			if (!components.empty())
			{
				components.copyToVector(this->mElements, true);
			}
		}

		/**
		*  @brief
		*    Constructor to pass a vector of components, which will be copied
		*/
		inline explicit ComponentsRandomEnumerator(const std::vector<T*>& inputToCopy, uint32 maxTries = std::numeric_limits<uint32>::max()) :
			RandomEnumerator<T>(inputToCopy, maxTries)
		{
			// Nothing here
		}

		// This constructor must not be used, see comment inside
		inline explicit ComponentsRandomEnumerator(std::vector<T*>& inputToCopy, uint32 maxTries = std::numeric_limits<uint32>::max())
		{
			// If you get there, you used a ComponentsRandomEnumerator constructor with a non-const std::vector reference as first argument.
			// In this case, use the constructor with "constructionpolicy::Policy" argument to specify whether to copy or swap contents with the given std::vector.
			// Both may be beneficial depending on your case:
			//  - constructionpolicy::COPY is safer, but produces overhead due to copying of data
			//  - constructionpolicy::SWAP will swap contents and consequently leave the given std::vector empty afterwards, which is fine if it is not used afterwards any more
			T::Used_ambiguous_ComponentsRandomEnumerator_constructor;
		}

		/**
		*  @brief
		*    Constructor to pass a vector of components, with the option to swap contents
		*/
		inline ComponentsRandomEnumerator(constructionpolicy::Policy policy, std::vector<T*>& inputToSwapWith, uint32 maxTries = std::numeric_limits<uint32>::max()) :
			RandomEnumerator<T>(policy, inputToSwapWith, maxTries)
		{
			// Nothing here
		}

		/**
		*  @brief
		*    Randomly choose a component that was not chosen already, and return its entity
		*
		*  @return
		*    Pointer to another entity, which gets randomly chosen from the list; or a null pointer in case we already had all components
		*/
		inline qsf::Entity* getNextEntity()
		{
			qsf::Component* component = this->getNext();
			return (nullptr == component) ? nullptr : &component->getEntity();
		}


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
