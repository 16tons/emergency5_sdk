// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/RouterPath.h"
#include "qsf_ai/navigation/em4Router/em2012/RouteFinder/routefindertypes.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class EGenericObject;
		class ERouterPath;
		class Bezier;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		class PathSmoothing : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			PathSmoothing();
			virtual ~PathSmoothing();

			/**
			*  @brief
			*    Init smoothing
			*
			*  @param[in] routerPath
			*    The em4 path we want to smooth
			*
			*  @param[in] object
			*    EGenericObject of the "qsf::Entity" we want to move along a path
			*    TODO(tl): we need to ensure that our EGenericObject is viable for the entire usage (Weakptr could be more save)
			*
			*  @param[in] flags
			*    EM4 routerflags, can for example be ERF_NONE, ERF_NO_BACKWARDS, ERF_ONLY_DIRECT_WAY, ERF_NOCURVES
			*/
			void initSmoothing(const ERouterPath& routerPath, EGenericObject* object, ERouterFlags flags);

			/**
			*  @brief
			*    Do a part of the Smoothing, can be called multiple times to optimize the path
			*
			*  @param[in] distance
			*    distance the object has already traveled on the path, 0 if the object hasn't started to move yet
			*
			*  @param[in] steps
			*    number of calculation steps we want to optimize the path. 0 to smooth the entire path
			*/
			void doSmoothing(float distance, int steps);

			/**
			*  @brief
			*    Try to include a new start node
			*
			*  @param[in] pos
			*    Position of the start node
			*
			*  @param[in] dir_
			*    direction of the start node
			*
			*  @param[in] movement_
			*    moving backwards or forwards, look at EMoveTypes for more information
			*
			*  @result
			*    if the including of the start node succeeded
			*/
			bool tryIncludingStartNode(const glm::vec3& pos_, float dir_, char movement_);

			/**
			*  @brief
			*    Return the smoothed path at this point
			*/
			ERouterPath& getPath();


			//[-------------------------------------------------------]
			//[ Private methods                                       ]
			//[-------------------------------------------------------]
		private:
			bool tryFindShortcut(size_t sourceNode_, size_t targetNode_, Bezier& curve_, char& movement_);
			bool tryFindShortcut(const ERouterPathNode& sourceNode_, size_t targetNode_, Bezier &curve_, char &movement_);
			bool tryFindShortcut(const ERouterPathNode& sourceNode_, const ERouterPathNode& targetNode_, Bezier &curve_, char &movement_);


			//[-------------------------------------------------------]
			//[ Private data                                          ]
			//[-------------------------------------------------------]
		private:
			ERouterPath mRouterPath;
			EGenericObject* mObject;
			ERouterFlags mPathFlags;
			size_t mSmoothFrom;
			size_t mSmoothTo;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
