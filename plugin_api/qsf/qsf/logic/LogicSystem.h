// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/job/JobProxy.h"
#include "qsf/math/EnumBitmask.h"
#ifdef QSF_PROFILING
	#include "qsf/logic/profiling/ProfilingData.h"
#endif


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
}


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
	*    Abstract logic system base class
	*
	*  @remarks
	*    Extends the system class by adding a hashed name as unique ID and a job registration to get regular update calls.
	*    Also, this class cares for logging and provides profiling for derived system.
	*/
	class QSF_API_EXPORT LogicSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] name
		*    The name of this logic system, it gets hashed to a unique ID
		*/
		inline explicit LogicSystem(const char* name);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LogicSystem();

		/**
		*  @brief
		*    Return the unique ID, which simply is the hashed name
		*/
		inline uint32 getId() const;

		/**
		*  @brief
		*    Return whether or not the logic system is currently in simulating mode
		*
		*  @return
		*    "true" if the logic system is currently in simulating mode, else "false"
		*/
		inline bool isSimulating() const;

		/**
		*  @brief
		*    Set the logic system and all running components inside to simulating mode
		*/
		void setSimulatingMode(bool simulatingMode);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::LogicSystem methods            ]
	//[-------------------------------------------------------]
	protected:
		virtual uint32 getJobManagerId() = 0;
		virtual void updateLogicSystem(const JobArguments& jobArguments) = 0;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Enable/disable the job
		*/
		void setJobActive(bool active);

		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*/
		void updateJob(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		#ifdef QSF_PROFILING
			ProfilingData mProfilingData;
		#endif


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		enum InternalFlags
		{
			SIMULATING_MODE,	///< If "true", the map is in simulating mode, which is a hint for components that we're not in the editor right now
			SERVER_MODE,		///< If "true", the map is in server mode, i.e. the application is running on the server
		};
		typedef EnumBitmask<uint8, InternalFlags> InternalFlagsSet;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		InternalFlagsSet mInternalFlags;	///< Internal map flags
		const uint32	 mHashedId;			///< Unique ID used as logging context
		const char*		 mName;
		JobProxy		 mUpdateJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/logic/LogicSystem-inl.h"
