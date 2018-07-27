// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/NavigationTask.h"
#include "qsf_ai/worldModel/dynamicUpdate/NavigationMapUpdateConfiguration.h"
#include "qsf_ai/worldModel/MapContactPoint.h"


namespace qsf
{
	namespace ai
	{
		class TrafficLaneWorld;

		/**
		* Instances of this class define the task of updating a traffic lane world that was created from converting a manually edited street network with a dynamic collision change.
		* Implemented as a navigation task that is scheduled by the NavigationTaskThread.
		*/
		class ConvertedStreetNetworkUpdateTask : public NavigationTask
		{
		public:
			ConvertedStreetNetworkUpdateTask(unsigned int mapId, const NavigationMapUpdateConfiguration& config);

			// public navigation task interface implementation
			//@{
			virtual process::State getState() const override;
			virtual process::State execute() override;
			virtual void interrupt() override;
			virtual void fail() override;
			//@}

		private:
			// Extracted parts of the main execute logic for modes add and remove respectively
			void addObstacle(const MapContactPoint& contactPoint);
			void removeObstacle(const MapContactPoint& contactPoint);

			process::State mState;
			const TrafficLaneWorld* mNavigationMapToUpdate; // will be initialized only during the execution step

			// static configuration data
			unsigned int mMapId;
			NavigationMapUpdateConfiguration mUpdateConfig;
		};
	}
}
