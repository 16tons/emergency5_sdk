// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/feeder/InputDeviceFeeder.h"
#include "qsf/input/InputListener.h"

#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class UserInputDevice;
	class InputControl;
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
	*    User input device feeder class
	*/
	class QSF_API_EXPORT UserInputDeviceFeeder : public InputDeviceFeeder, public InputListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		UserInputDeviceFeeder();

		/**
		*  @brief
		*    Destructor
		*/
		~UserInputDeviceFeeder();

		/**
		*  @brief
		*    Add a new connection from a user-defined input control to a source input control.
		*
		*  @remarks
		*    It's okay to add connections that already exist, but you may not add circular connections between multiple controls (that does not make sense anyway).
		*/
		void addConnection(InputControl& userControl, InputControl& sourceControl);

		/**
		*  @brief
		*    Remove the connection from a user-defined input control to a source input control.
		*
		*  @remarks
		*    It's okay to remove connections that don't exist.
		*/
		void removeConnection(InputControl& userControl, InputControl& sourceControl);

		/**
		*  @brief
		*    Checks whether or not the two given controls are already connected.
		*/
		bool isConnected(InputControl& userControl, InputControl& sourceControl, bool recursive) const;

		/**
		*  @brief
		*    This method gets called after an event was added to the input system's event queue
		*/
		void onEventAdded(const InputEvent& inputEvent);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputDeviceFeeder methods         ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Callback before a device gets removed
		*/
		virtual void onDeviceRemoval(InputDevice& inputDevice) override;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		typedef std::multimap<InputControl*, InputControl*> ConnectionMap;
		ConnectionMap mConnections;		///< Maps source input controls to (user-defined) input controls

		typedef std::pair<ConnectionMap::const_iterator, ConnectionMap::const_iterator> ConnectionEqualRange;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
