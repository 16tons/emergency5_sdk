// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Plugin jobs collection base class
	*
	*  @remarks
	*    The QSF job system consists of a collection of job managers. Each job manager might be inside another
	*    job manager. Each job manager might have multiple job proxies assigned to it. When registering a job
	*    proxy, the job manager is identified using an ID.
	*
	*    In order to avoid chaos regarding to figuring out which job managers exist, we introduced this jobs
	*    collection class. Each plugin which registers new job managers should do this inside classes derived
	*    from this "qsf::Jobs" base class.
	*
	*    QSF provides the following standard job manager hierarchy:
	*    - Default:       The root, provides the child job manager with continuous standard timing
	*      - Realtime:    Is using the continuous timing as provided by the root without manipulating it. Typical use cases are:
	*                     renderer, input, GUI, debug draw, network (client/server)
	*      - Animation:   Modifies the continuous timing received by the default job manager. Typical use cases are:
	*                     mesh, particles
	*      - Simulation:  Modifies the continuous timing received by the default job manager and provides job proxies with fixed
	*                     ticks instead of continuous timing. Typical use cases are:
	*                     time of day, weather, physics, fire, events, sequencer, AI
	*
	*    Beside the differences in timing, the job manager hierarchy can also be used to easily control which job proxies receive
	*    updates. For example, inside the editor only realtime job managers are updated, but it's possible to start e.g. particles
	*    animation on demand.
	*
	*    All plugin job manager collections should be derived from this class. This makes it easier
	*    to find all classes within e.g. the automatic code documentation.
	*/
	class QSF_API_EXPORT Jobs : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		inline Jobs();

		/**
		*  @brief
		*    Destructor
		*
		*  @brief
		*    - Not virtual by intent
		*/
		inline ~Jobs();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/job/Jobs-inl.h"
