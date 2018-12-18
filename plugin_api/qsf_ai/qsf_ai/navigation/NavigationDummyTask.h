// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationTask.h"


namespace qsf
{
	namespace ai
	{
		/**
		* A no op dummy task implementation of the abstract navigation task interface to be used as key during searches inside collections of real tasks for example.
		*/
		class NavigationDummyTask : public NavigationTask
		{
		public:
			NavigationDummyTask(unsigned int taskId);

			// public NavigationTask interface implementation
			//@{
			virtual process::State getState() const override;
			virtual process::State execute() override;
			virtual void interrupt() override;
			virtual void fail() override;
			//@}
		};

		// inline implementation
		inline NavigationDummyTask::NavigationDummyTask(unsigned int taskId) :
			NavigationTask(NavigationTask::DUMMY, taskId)
		{}

		inline process::State NavigationDummyTask::getState() const
		{
			return process::FINISHED; // pretty arbitrary
		}

		inline process::State NavigationDummyTask::execute()
		{
			return process::FINISHED; // pretty arbitrary again
		}

		inline void NavigationDummyTask::interrupt()
		{}

		inline void NavigationDummyTask::fail()
		{}
	}
}
