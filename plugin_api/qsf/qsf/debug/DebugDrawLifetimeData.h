// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/StringHash.h"
#include "qsf/time/Time.h"

#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>


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
	*    Encapsulates the common life time date for debug draw requests
	*/
	class QSF_API_EXPORT DebugDrawLifetimeData : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor creating an unlimited lifetime setting
		*/
		inline DebugDrawLifetimeData();

		/**
		*  @brief
		*    Construct with a given duration
		*/
		inline explicit DebugDrawLifetimeData(Time duration);

		/**
		*  @brief
		*    Construct with a given duration, and job manager ID to force registration at a specific job manager
		*/
		inline explicit DebugDrawLifetimeData(Time duration, const StringHash& jobManagerId);

		/** Return true if there is lifetime duration set at all */
		inline bool isDurationLimited() const;

		/** Return lifetime duration; throws an exception if the duration is not limited */
		inline Time getDuration() const;

		/** Return the job manager ID where the job managing the lifetime is registered */
		inline uint32 getJobManagerId() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// TODO(vs) We could also use std::numeric_limits<float>::infinity to signal unlimited duration
		// TODO(vs) Use Unsigned Time because negative limitations don't make any sense
		boost::optional<Time> mDuration;		///< Lifetime duration in seconds, uninitialized means no limitation
		uint32				  mJobManagerId;	///< ID of job manager where the lifetime update job gets registered


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/debug/DebugDrawLifetimeData-inl.h"
