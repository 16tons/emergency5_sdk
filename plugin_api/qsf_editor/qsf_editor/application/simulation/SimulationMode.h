// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/map/backup/BinaryMapBackup.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Editor simulation mode base class
		*/
		class SimulationMode : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			virtual ~SimulationMode() {}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::SimulationMode methods    ]
		//[-------------------------------------------------------]
		public:
			virtual void startupSimulation() = 0;
			virtual void shutdownSimulation() = 0;


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			void backupMap();
			void restoreMap();
			void setJobManagerAutoUpdate(uint32 jobManagerId, bool enable);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			BinaryMapBackup mMapBackup;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
