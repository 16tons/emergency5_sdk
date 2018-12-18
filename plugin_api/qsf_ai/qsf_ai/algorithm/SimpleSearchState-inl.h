// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/ErrorHandling.h>
#include <qsf/log/LogSystem.h>

namespace qsf
{
	namespace ai
	{
		template <typename Costs>
		SimpleSearchState<Costs>::SimpleSearchState() :
			mCostsToGetHere(Cost::getUninitialized()),
			mCostsToTarget(Cost::getUninitialized()),
			mIsExpanded(false)
		{}

		template <typename Costs>
		operation::Indicator SimpleSearchState<Costs>::onPathFound(const boost::optional<std::size_t>& predecessorId, Cost costsToGetHere, bool usesLateStart)
		{
			QSF_CHECK(Cost::isValid(costsToGetHere), "Providing invalid cost value on path found", QSF_REACT_THROW);

			if (mIsExpanded)
			{
				// TODO(vs) Make this a warning again.
				// We get these errors with the admittedly tricky heuristic funnel smoothed heuristic for traffic lanes and sadly I don't have the time to debug it right now.
				if (!usesLateStart && (costsToGetHere < mCostsToGetHere))
					QSF_LOG_PRINTS(TRACE, "found cheaper path after expansion, values are " << *mCostsToGetHere << " and " << *costsToGetHere);

				return operation::NO_OP;
			}

			if (!Cost::isValid(mCostsToGetHere) || costsToGetHere < mCostsToGetHere)
			{
				mCostsToGetHere = costsToGetHere;
				mPredecessorId = predecessorId;
				return Cost::isValid(mCostsToTarget) ? operation::SCHEDULE : operation::ESTIMATE_AND_SCHEDULE;
			}

			return operation::NO_OP;
		}

		template <typename Costs>
		void SimpleSearchState<Costs>::setEstimatedCostsToTarget(Cost costs)
		{
			QSF_WARN_IF(Cost::isValid(mCostsToTarget), "Assigning estimated costs a second time may lead to incorrect search results", QSF_REACT_NONE);
			QSF_CHECK(Cost::isValid(costs) || (*costs).getValue() == std::numeric_limits<float>::max(), "Trying to set invalid costs of " << (*costs).getValue(), QSF_REACT_THROW);

			mCostsToTarget = costs;
		}

		template <typename Costs>
		Costs SimpleSearchState<Costs>::getTotalCosts() const
		{
			QSF_CHECK_ONCE(Cost::isValid(mCostsToGetHere), "Accessing uninitialized costs from start", QSF_REACT_THROW);
			QSF_CHECK_ONCE(Cost::isValid(mCostsToTarget), "Accessing uninitialized costs to goal", QSF_REACT_THROW);

			return mCostsToGetHere + mCostsToTarget;
		}

		template <typename Costs>
		Costs SimpleSearchState<Costs>::getCostsToGetHere() const
		{
			return mCostsToGetHere;
		}

		template <typename Costs>
		Costs SimpleSearchState<Costs>::getEstimatedCosts() const
		{
			return mCostsToTarget;
		}

		template <typename Costs>
		void SimpleSearchState<Costs>::setExpanded()
		{
			QSF_WARN_IF(mIsExpanded, "flagging an already expanded node as expanded again may indicate an algorithmic error",
				return);

			mIsExpanded = true;
		}

		template <typename Costs>
		bool SimpleSearchState<Costs>::isExpanded() const
		{
			return mIsExpanded;
		}

		template <typename Costs>
		bool SimpleSearchState<Costs>::wasReached() const
		{
			return Costs::isValid(mCostsToGetHere);
		}

		template <typename Costs>
		const boost::optional<std::size_t>& SimpleSearchState<Costs>::getPredecessorId() const
		{
			return mPredecessorId;
		}
	}
}
