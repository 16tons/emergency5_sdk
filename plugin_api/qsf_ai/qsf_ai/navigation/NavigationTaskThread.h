// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/pathfinding/PathSearch.h"
#include "qsf_ai/navigation/NavigationTaskStatistics.h"

#include <qsf/component/Component.h>
#include <qsf/reflection/CampDefines.h>

#include <thread>
#include <memory>
#include <vector>
#include <condition_variable>

namespace qsf
{
	namespace ai
	{
		class Path;

		/**
		* The navigation task thread runs long running navigation system tasks in a separate thread.
		* It is a support system for other AI navigation systems.
		* These are currently path searches and navigation map updates.
		* It is expected to be added as a core component to the core entity but it is not an AI standard systems as its update logic is different.
		*/
		class NavigationTaskThread : public Component
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			NavigationTaskThread(Prototype* prototype);
			~NavigationTaskThread();

			// public component interface implementation
			//@{
			virtual bool onStartup() override;
			virtual void onShutdown() override;
			virtual void onSetSimulating(bool simulating) override;
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// This triggers a path search and returns a unique ID with which to query for the state of the running search.
			// The search results stays persistent and occupies memory until it is retrieved via fetchResult(id).
			// This sets up the search but doesn't do any actual searching work.
			// The search takes ownership on the search configuration object passed, the pointer is reset during the call
			unsigned int requestSearch(std::auto_ptr<PathSearchConfiguration> config);

			/**
			* Tries to retrieve the result for the search with the passed id.
			* Writes the resulting path to the reference parameter in case the search was a success.
			* The current transform of the client is necessary in cases it has moved after requesting the search to make some late changes during path smoothing that fit to changed situation.
			*/
			process::State tryFetchResult(unsigned int searchId, const Transform& currentTransform, Path& path, bool debugState);

			// Stops and joins in the wrapped thread after the current task is finished
			void scheduleShutdown();

			// Cancel a running search.
			// Use the id that was returned when the search was started.
			// It is legal to cancel a search that is succeeded or failed but canceling an already canceled or otherwise invalid search id will issue a warning.
			void cancelSearch(unsigned int id);

			// Interrupt a current search and try to calculate the next search
			// We should call this function in certain timesteps to prevent tasks from blocking the system
			void interruptSearch();

			// Access is not thread safe. It is assumed that even concurrent access to this data is not critical because it is only used for debugging purposes
			const NavigationTaskStatistics& getNavigationTaskStatistics() const;

			// request a new map update task
			void requestMapUpdate(std::auto_ptr<NavigationTask> updateTask);

		private:
			typedef std::deque<PathSearch*> SearchQueue;

			// Main update function that executes tasks in the background
			void executeTasks();

			// memory cleanup during destructor
			void resetSearchQueue(SearchQueue& queue); // frees all memory for one search queue
			void cleanUpPendingSearches(); // remove searches, keep map updates intact
			void cleanUpPendingTasks(); // remove searches and map updates

			// Serializing helper function
			PathSearch* createPathSearch(unsigned int taskId, std::auto_ptr<PathSearchConfiguration> searchConfig);
			void serializeNavigationTask(NavigationTask*& task, BinarySerializer& serializer);
			void serializeSearchRequest(PathSearch*& request, BinarySerializer& serializer);
			void serializeSearchQueue(SearchQueue& queue, BinarySerializer& serializer);

			// Pops and return the next pending search from the queue. Returns a dummy with a search nullptr if there is no pending search
			NavigationTask* fetchNextPendingRequest();
			void transferCurrentToFinished();

			// All the running searches are stored in here. The memory is handled manually meaning that you need to destroy objects when they are removed from the container.
			SearchQueue mPendingRunningSearches; // holds all scheduled and halfway calculated searches for round robin
			SearchQueue mPendingRegularSearches; // holds all scheduled not yet calculated regular searches
			SearchQueue mPendingPrioritySearches; // holds all scheduled not yet calculated priority searches
			NavigationTask* mCurrentlyHandledTask; // holds the currently examined search
			SearchQueue mFinishedSearches; // holds all calculated search results

			// Map Update task data
			std::deque<NavigationTask*> mPendingMapUpdates;

			unsigned int mNextTaskId;
			bool mFailCurrentTask;			// Special flag for the case that the a task is canceled while it is being executed.
			bool mInterruptedTask;			// Special flag for the case that the a task was interrupted while it is being executed.
			bool mIgnoreNextInterrupt;	// Increase this value to ignore the next interrupts

			NavigationTaskStatistics mTaskStatistics;

			// Threading control
			std::auto_ptr<std::thread> mCalculationThread;
			mutable std::mutex		   mCommunicationMutex; // For synchronization writes and reads between the calculation and requesting threads
			std::condition_variable    mActivityTrigger;	// Variable which triggers whether the thread is currently active
			bool					   mRunning;			// Flag to avoid running into a dead lock situation

			QSF_CAMP_RTTI();
		};
	}
}

QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::NavigationTaskThread);
