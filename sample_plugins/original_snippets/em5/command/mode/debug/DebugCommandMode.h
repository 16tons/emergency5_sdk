// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf_game/command/mode/CommandMode.h>

#include <qsf/reflection/CampClass.h>	// This include is not needed here directly, but most likely in a derived class
#include <qsf/debug/DebugDrawProxy.h>	// This include is not needed here directly, but most likely in a derived class


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Debug command mode abstract base class
	*
	*  @note
	*    - This class is only a structure element, plus some protected helper methods
	*/
	class EM5_API_EXPORT DebugCommandMode : public qsf::game::CommandMode
	{


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		/**
		*  @brief
		*    Default constructor
		*/
		DebugCommandMode(uint32 commandId);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DebugCommandMode();

		/**
		*  @brief
		*    Get the (first) currently running freeplay event, or a null pointer if there is none
		*/
		FreeplayEvent* getCurrentFreeplayEvent() const;

		/**
		*  @brief
		*    Get the current mouse position
		*/
		glm::vec2 getMousePosition() const;

		/**
		*  @brief
		*    Get the current normalized mouse position
		*/
		glm::vec2 getNormalizedMousePosition() const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
