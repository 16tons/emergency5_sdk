// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/job/JobProxy.h>
#include <qsf/debug/DebugDrawProxy.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class RenderWindow;
}


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
		*    Compass visualization class
		*/
		class Compass : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] renderWindow
			*    Render window to use, the instance must stay alive as long as this compass visualization exists
			*/
			explicit Compass(RenderWindow& renderWindow);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~Compass();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateJob(const JobArguments&);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			RenderWindow&  mRenderWindow;		///< Render window to use, always valid, to not destroy the instance
			JobProxy	   mJobProxy;			///< Job proxy for regular updates
			DebugDrawProxy mDebugDrawProxy;		///< Debug draw proxy for the visualization


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
