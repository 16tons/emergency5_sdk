// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/SimpleSearchState.h"

#include <cstddef>

namespace qsf
{
	namespace ai
	{
		/** An IndexedSearchState is a search state variant that carries an index into some other data structure.
		* @note The superclass SimpleSearchState has no virtual destructor to avoid a vtable pointer per object.
		* Don't dynamically create these objects and delete them via a superclass pointer.
		* Noncopyable by inheritance.
		*/
		template <typename Costs>
		class IndexedSearchState : public SimpleSearchState<Costs>
		{
		public:
			typedef Costs Cost; // necessary typedef to keep using the costs in weighted search states and with the AStar implementation

			IndexedSearchState(std::size_t index);
			std::size_t getIndex() const;

		private:
			std::size_t mIndex;
		};


		// inline implementation
		template <typename Costs>
		IndexedSearchState<Costs>::IndexedSearchState(std::size_t index) :
			mIndex(index)
		{}

		template <typename Costs>
		std::size_t IndexedSearchState<Costs>::getIndex() const
		{
			return mIndex;
		}
	}
}
