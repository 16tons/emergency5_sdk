// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/algorithm/OperationIndicator.h"

#include <boost/optional.hpp>

namespace qsf
{
	namespace ai
	{
		/** Example search state for the AStar search.
		* A search state may have a predecessor, current best costs found to reach it and an estimation towards the goal.
		* All these variables are optional
		* The costs are a template parameter that needs to comply to the Cost interface.
		*/
		template <typename Costs>
		class SimpleSearchState
		{
		public:
			typedef Costs Cost; // necessary typedef to keep using the costs in weighted search states and with the AStar implementation

			SimpleSearchState();

			//@{ template interface required by AStar search states
			// called when a new path is found and decides on its internal state what should be done as a reaction
			// The predecessor parameter might be uninitialized in case the path is found directly from the start.
			// See the AStar class comment about the usesLateStart option that is passed as a flag here
			operation::Indicator onPathFound(const boost::optional<std::size_t>& predecessorId, Cost costsToGetHere, bool usesLateStarts);
			void setEstimatedCostsToTarget(Cost costs);
			// the sum of costsToGetHere plus estimatedCostsToGoal
			Cost getTotalCosts() const;
			Cost getCostsToGetHere() const;
			Cost getEstimatedCosts() const;

			// Access to the flag whether a state was already expanded during the search and successor states were created.
			// This is tracked to avoid infinite loops with illegal cost configurations.
			//@{
			void setExpanded();
			bool isExpanded() const;
			//@}

			// A state is considered reached if at least one path was found towards it.
			bool wasReached() const;

			// Returns an uninitialized instance if there is no predecessor set by the AStar design
			const boost::optional<std::size_t>& getPredecessorId() const;
			//@}
		private:
			// noncopyable
			SimpleSearchState(const SimpleSearchState&);
			SimpleSearchState& operator=(const SimpleSearchState&);

			Cost mCostsToGetHere;
			Cost mCostsToTarget;
			boost::optional<std::size_t> mPredecessorId;
			bool mIsExpanded;
		};
	}
}

#include "qsf_ai/algorithm/SimpleSearchState-inl.h"
