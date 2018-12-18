// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


namespace qsf
{
	namespace ai
	{
		inline Path::Path(const std::deque<Waypoint>& nodes) :
			mState(UNDER_CONSTRUCTION),
			mNodes(nodes),
			mCurrentNode(0),
			mReservationRange(0, 0),
			mMayUseRouterAdaptation(true),
			mMayRestartSearch(false)
		{}

		inline Path::State Path::getState() const
		{
			return mState;
		}

		inline const Waypoint& Path::getCurrentNode() const
		{
			QSF_CHECK(mCurrentNode < mNodes.size(), "index out of bounds when accessing path node: " << mCurrentNode << " >= " << mNodes.size(), QSF_REACT_THROW);

			return mNodes[mCurrentNode];
		}

		inline Waypoint& Path::getWaypoint(unsigned int index)
		{
			QSF_CHECK(index < mNodes.size(), "index out of bounds when accessing path node: " << mCurrentNode << " >= " << mNodes.size(), QSF_REACT_THROW);

			return mNodes[index];
		}

		inline const Waypoint& Path::getWaypoint(unsigned int index) const
		{
			QSF_CHECK(index < mNodes.size(), "index out of bounds when accessing path node: " << mCurrentNode << " >= " << mNodes.size(), QSF_REACT_THROW);

			return mNodes[index];
		}

		inline unsigned int Path::getCurrentNodeIndex() const
		{
			return mCurrentNode;
		}

		inline const std::deque<Waypoint>& Path::getNodes() const
		{
			return mNodes;
		}

		inline void Path::finishConstruction()
		{
			QSF_WARN_IF(mState != UNDER_CONSTRUCTION && mState != NEEDS_ADAPTATION && mState != RUN_AND_ADAPT, "Unable to end the path editing, path is in state " << mState,
				return);

			mState = mNodes.empty() ? FINISHED : RUNNING; // an empty path is immediately finished

			//TODO(bs) this is a bit of an ugly hack. currently, we mess with the path in order to remove unwanted segments.
			cleanUp();
		}

		inline void Path::requestAdaptation()
		{
			QSF_CHECK(mState != FINISHED && mState != UNDER_CONSTRUCTION, "Unable to request adaptation to path in state " << mState,
				return);

			mState = NEEDS_ADAPTATION;
		}

		inline void Path::requestRunAndAdapt()
		{
			QSF_CHECK(mState != FINISHED && mState != UNDER_CONSTRUCTION, "Unable to request adaptation to path in state " << mState,
				return);

			mState = RUN_AND_ADAPT;
		}

		inline void Path::forceAdaptationRequest()
		{
			mState = NEEDS_ADAPTATION;
		}

		inline bool Path::isInClearedState() const
		{
			return mNodes.empty() && mState == UNDER_CONSTRUCTION && mCurrentNode == 0;
		}

		inline void Path::popFront()
		{
			QSF_CHECK(!mNodes.empty(), "Unable to pop first element from empty path",
				return);

			mNodes.pop_front();
		}

		inline void Path::popBack()
		{
			QSF_CHECK(!mNodes.empty(), "Unable to pop last element from empty path",
				return);

			mNodes.pop_back();
		}

		inline void Path::addNodes(const std::deque<Waypoint>& newWPs)
		{
			for (auto iter = newWPs.begin(); iter != newWPs.end(); ++iter)
				addNode(*iter);
		}

		inline void Path::setReservationWindow(const IdRange& reservedIds)
		{
			mReservationRange = reservedIds;
		}

		inline const Path::IdRange& Path::getReservationWindow() const
		{
			return mReservationRange;
		}

		inline void Path::eraseNode(unsigned int position)
		{
			QSF_CHECK(position < mNodes.size(), "Index out of range, unable to erase path node " << position << " of " << mNodes.size() << " nodes",
				return);

			mNodes.erase(mNodes.begin() + position);
		}

		inline void Path::eraseNodesFromToPathEnd(unsigned int position)
		{
			QSF_CHECK(position < mNodes.size(), "Index out of range, unable to erase path nodes " << position << " of " << mNodes.size() << " nodes",
				return);

			mNodes.erase(mNodes.begin() + position, mNodes.end());
		}

		inline void Path::requestTryToFindAnAlternative()
		{
			QSF_CHECK(mState != FINISHED && mState != UNDER_CONSTRUCTION, "Unable to request an alternative in state " << mState,
				return);

			mState = TRY_ALTERNATIVE;
		}

		inline void Path::onNoAlternativeFound()
		{
			if (mState != TRY_ALTERNATIVE)
				return;

			mState = RUNNING;
		}

		inline Path::EvadedEntitiesList& Path::getCurrentlyEvadedEntities()
		{
			return mCurrentlyEvadedEntities;
		}

		inline void Path::setMayUseRouterPathAdaptation(bool enable)
		{
			mMayUseRouterAdaptation = enable;
		}

		inline bool Path::mayUseRouterPathAdaptation() const
		{
			return mMayUseRouterAdaptation;
		}

		inline void Path::onPartialLocalRouterResultIntegrated()
		{
			mMayRestartSearch = true; // doesn't matter if this happens several times on one path - we always only need to restart once
		}

		inline bool Path::mayRestartSearch() const
		{
			return mMayRestartSearch;
		}
	}
}
