// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class InputDevice;
	class InputEvent;
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
	*    Input device feeder base class
	*/
	class QSF_API_EXPORT InputDeviceFeeder
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline InputDeviceFeeder();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InputDeviceFeeder();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputDeviceFeeder methods         ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Feed device
		*/
		inline virtual void updateDeviceFeeder();

		/**
		*  @brief
		*    Callback before a device gets removed
		*/
		inline virtual void onDeviceRemoval(InputDevice& inputDevice);


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	protected:
		void pushBackInputEvent(InputEvent* inputEvent);


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/feeder/InputDeviceFeeder-inl.h"
