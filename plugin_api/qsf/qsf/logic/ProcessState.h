// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace process
	{


		// Reusable enum defining the state of some long running process that may be updated several times but is bound to end eventually
		enum State
		{
			WAITING,		///< Not started yet
			INTERRUPTED,	///< Started, but currently not running
			RUNNING,		///< Started and currently running
			FINISHED,		///< Finished successfully
			FAILED			///< Finished with failure
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // process
} // qsf
