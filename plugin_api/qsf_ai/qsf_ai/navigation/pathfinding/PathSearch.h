// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/navigation/pathfinding/PathSearchConfiguration.h"
#include "qsf_ai/navigation/NavigationTask.h"

#include <qsf/time/Time.h>

#include <memory>


namespace qsf
{
	class CompoundDebugDrawRequest;

	namespace ai
	{
		class Path;

		/** A running search instance of the path finding system.
		* This is only the abstract superclass for the actual implementations based on different world models.
		*/
		class QSF_AI_API_EXPORT PathSearch : public NavigationTask
		{
		public:
			// The path search takes ownership of the passed configuration, the pointer is reset during the call
			PathSearch(unsigned int taskId, std::auto_ptr<PathSearchConfiguration> searchConfig);

			// Write the path found into the reference parameter.
			// Does nothing if the search was not successful.
			// Receives the current transform in case the entity might have moved since the planning time to make some minor corrections during smoothing
			virtual void writeResultingPath(const Transform& currentTransform, Path& path) const = 0;

			// Used for profiling
			virtual std::size_t getNumExpandedSearchStates() const = 0;

			PathSearchConfiguration& getSearchConfiguration();

			// Create debug output describing the search state on the compound object
			virtual void createStateDebug(CompoundDebugDrawRequest& cddr) const = 0;

			// The time data is only tracked in case QSF is compiled in profiling mode (QSF_PROFILING defined).
			void setTimeUsed(Time time);
			Time getTimeUsed() const;

		protected:
			std::auto_ptr<PathSearchConfiguration> mSearchConfig;

			Time mTimeUsed; // This is usually initialized after the search to track the time needed for the planning mainly for statistical uses
		};
	}
}

#include "qsf_ai/navigation/pathfinding/PathSearch-inl.h"
