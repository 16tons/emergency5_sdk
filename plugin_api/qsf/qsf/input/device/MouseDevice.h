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
#include "qsf/input/control/InputAxis.h"

#include <glm/glm.hpp>


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
	*    Mouse input device class
	*/
	class QSF_API_EXPORT MouseDevice : public InputDevice
	{


	//[-------------------------------------------------------]
	//[ Device definition                                     ]
	//[-------------------------------------------------------]
	public:
		InputAxis	XAxis;			///< X axis (absolute position)
		InputAxis	YAxis;			///< Y axis (absolute position)
		InputAxis	XMove;			///< X axis (movement data)
		InputAxis	YMove;			///< Y axis (movement data)
		InputAxis	Wheel;			///< Mouse wheel (movement data)
		InputButton	Left;			///< Left mouse button (mouse button #1)
		InputButton	Right;			///< Right mouse button (mouse button #2)
		InputButton	Middle;			///< Middle mouse button (mouse button #3)
		InputButton	Button4;		///< Mouse button #4
		InputButton	Button5;		///< Mouse button #5
		InputButton	Button6;		///< Mouse button #6
		InputButton	Button7;		///< Mouse button #7
		InputButton	Button8;		///< Mouse button #8
		InputButton	Button9;		///< Mouse button #9
		InputButton	Button10;		///< Mouse button #10
		InputButton	Button11;		///< Mouse button #11
		InputButton	Button12;		///< Mouse button #12
		bool		mPositionValid;	///< "false" if the mouse position is not valid (this is the case at program startup only)


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		MouseDevice();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MouseDevice();

		/**
		*  @brief
		*    Return current mouse position as glm::vec2
		*/
		glm::vec2 getPosition() const;

		/**
		*  @brief
		*    Return current mouse position as glm::ivec2
		*/
		glm::ivec2 getPositionIvec2() const;

		/**
		*  @brief
		*    Return current mouse movement as glm::vec2
		*/
		glm::vec2 getMovement() const;

		/**
		*  @brief
		*    Return current mouse movement as glm::ivec2
		*/
		glm::ivec2 getMovementIvec2() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputDevice methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void updateDevice();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
