// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/platform/PlatformTypes.h>

#include <boost/noncopyable.hpp>

#include <vector>

namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Helper to get Entity locks for all entities which have an TrafficLightComponent
		*/
		class TrafficLightLockHelper : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			TrafficLightLockHelper();

			bool tryLockAll();
			void unLockAll();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<uint64>	mLockedTrafficLightsEntities;	//<< List of entity ids with traffic light components


		};
	}
}
