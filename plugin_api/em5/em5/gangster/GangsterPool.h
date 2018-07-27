// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	/**
	*  @brief
	*    Defines pools of gangster prefabs needed for a events.
	*    Only prefabs from the resp. pool are picked for new gangster entities.
	*/
	enum class GangsterPool
	{
		DEFAULT = 0,		///< ("List A") Used when usual gangster types are needed (e.g. pickpocket, robbery, ...)
		DEFAULT_EXTENDED,	///< ("List B") The ones from default + several male civil types (e.g. for running amok, arsonist, ...)
		TERRORIST,			///< ("List D") Used for terrorist gangster events (e.g. bomb planter)
		CONVICT				///< ("List E") Used for gangsters on the run
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
