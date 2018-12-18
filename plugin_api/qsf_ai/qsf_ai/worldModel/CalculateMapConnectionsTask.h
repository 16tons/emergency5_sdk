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
		* Task to calculate the connections between two maps.
		*/
		class CalculateMapConnectionsTask : public NavigationTask
		{
		public:
			CalculateMapConnectionsTask(unsigned int mapAId, unsigned int mapBId);

			// Implemented NavigationTask public interface
			//@{
			virtual process::State getState() const;
			virtual process::State execute();
			virtual void interrupt();
			virtual void fail();
			//@}

		private:
			process::State mState;
			unsigned int mMapAId;
			unsigned int mMapBId;
		};
	}
}
