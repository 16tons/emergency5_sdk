// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/GetUninitialized.h>
#include <qsf/logic/ProcessState.h>
#include <qsf/time/Time.h>

#include <boost/noncopyable.hpp>


namespace qsf
{
	namespace ai
	{
		/**
		* Abstract superclass for background tasks that are scheduled inside the AI navigation system.
		* The navigation task thread is the one that will execute these.
		*/
		class NavigationTask : public boost::noncopyable
		{
		public:
			// The known types of navigation tasks
			enum Type
			{
				DUMMY,
				PATH_SEARCH,
				VORONOI_REINIT, // reinitialize the voronoi graph initially after loading
				VORONOI_COLLISION_UPDATE, // integrate a dynamic collision into  the voronoi graph
				SIMPLE_COLLISION_UPDATE, // integrate a dynamic collision into a regular map structure
				MAP_CONNECTION,
			};

			// Convert to printable debug output
			static const char* getDebugOutputForType(Type type);

			explicit NavigationTask(Type type, unsigned int id = getUninitialized<unsigned int>());
			virtual ~NavigationTask();

			/**
			* Query the state of the task
			*/
			virtual process::State getState() const = 0;

			/** Main function to execute the task.
			* Returns the resulting state of the search.
			*/
			virtual process::State execute() = 0;

			/**
			* Interrupt the task, used for multi threaded execution.
			*/
			virtual void interrupt() = 0;

			/**
			* Forced failing a task from the outside.
			*/
			virtual void fail() = 0;

			unsigned int getRequestId() const;
			Type getType() const;

			/*
			* The time this task did exist until now
			*/
			inline Time getCurrentRunningTime() const;

		protected:
			Type mType;
			unsigned int mRequestId; // may remain uninitialized in case no one wants to retrieve some kind of result

			Time mStartingTimeStamp;
		};


		// inline implementation
		inline NavigationTask::NavigationTask(Type type, unsigned int id) :
			mType(type),
			mRequestId(id),
			mStartingTimeStamp(Time::now())
		{}

		inline NavigationTask::~NavigationTask()
		{}

		inline unsigned int NavigationTask::getRequestId() const
		{
			return mRequestId;
		}

		inline NavigationTask::Type NavigationTask::getType() const
		{
			return mType;
		}

		inline Time NavigationTask::getCurrentRunningTime() const
		{
			return Time::now() - mStartingTimeStamp;
		}
	}
}
