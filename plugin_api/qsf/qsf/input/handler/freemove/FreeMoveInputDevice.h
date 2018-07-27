// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/device/InputDevice.h"
#include "qsf/input/control/InputButton.h"


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
	*    WASD input device class
	*/
	class QSF_API_EXPORT FreeMoveInputDevice : public InputDevice
	{


	//[-------------------------------------------------------]
	//[ Device definition                                     ]
	//[-------------------------------------------------------]
	public:
		InputButton Left;		///< Left
		InputButton Right;		///< Right
		InputButton Forward;	///< Forward
		InputButton Backward;	///< Backward
		InputButton Upward;		///< Upward
		InputButton Downward;	///< Downward
		InputButton Slow;		///< Slow movement
		InputButton Fast;		///< Fast movement
		InputButton Rotate;		///< Rotate mode


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		FreeMoveInputDevice();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~FreeMoveInputDevice();

		/**
		*  @brief
		*    Return a movement vector from left/right/forward/backward/upward/downward input
		*/
		glm::vec3 getMoveVector() const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
