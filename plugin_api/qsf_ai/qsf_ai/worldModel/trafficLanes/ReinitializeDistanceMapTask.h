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
		* A navigation task to reinitialize the voronoi distance map after a navigation map has just been read from file or savegame.
		*/
		class ReinitializeDistanceMapTask : public NavigationTask
		{
		public:
			ReinitializeDistanceMapTask(unsigned int mapId);

			// public navigation task interface implementation
			//@{
			virtual process::State getState() const override;
			virtual process::State execute() override;
			virtual void interrupt() override;
			virtual void fail() override;
			//@}

		private:
			process::State mState;
			unsigned int mMapId;
		};
	}
}
