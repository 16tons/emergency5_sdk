// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <OIS/OISMouse.h> // Needed for the OIS::MouseButtonID. Forward declaration of an enum isn't legal as of the standard without specifying the underlying data type. See also http://stackoverflow.com/questions/71416/forward-declaring-an-enum-in-c

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace OIS
{
	class InputManager;
	class Keyboard;
}
namespace qsf
{
	class OisKeyboardFeeder;
	class OisMouseFeeder;
	class OisJoystickFeeder;
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
	*    OIS manager class
	*/
	class QSF_API_EXPORT OisManager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		OisManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OisManager();

		/**
		*  @brief
		*    Return the OIS keyboard device
		*
		*  @return
		*    The OIS keyboard device, can be a null pointer, do no destroy the returned instance, do only work with this pointer on the C runtime stack because the device instance may become invalid
		*/
		inline OIS::Keyboard* getOisKeyboard() const;

		/**
		*  @brief
		*    Return the OIS mouse device
		*
		*  @return
		*    The OIS mouse device, can be a null pointer, do no destroy the returned instance, do only work with this pointer on the C runtime stack because the device instance may become invalid
		*/
		inline OIS::Mouse* getOisMouse() const;

		/**
		*  @brief
		*    Return the OIS joystick device
		*
		*  @return
		*    The OIS joystick device, can be a null pointer, do no destroy the returned instance, do only work with this pointer on the C runtime stack because the device instance may become invalid
		*/
		inline OIS::JoyStick* getOisJoystick() const;

		/**
		*  @brief
		*    Check whether OIS instances were successfully created
		*/
		inline bool hasOisInstances() const;

		/**
		*  @brief
		*    Create the OIS-related class instances used internally
		*
		*  @return
		*    "true" if all went fine, "false" if not everything could be created
		*
		*  @remarks
		*    - Throws an exception if the OIS input manager instances could not be created for whatever reason
		*/
		bool createOisInstances(handle nativeWindowHandle, bool exclusive);

		/**
		*  @brief
		*    Destroy the OIS-related class instances used internally
		*
		*  @return
		*    "true" if all went fine, "false" if nothing had to be done
		*/
		bool destroyOisInstances();

		/**
		*  @brief
		*    Update the mouse clipping area
		*
		*  @note
		*    - Call this whenever the size of the focus native window handle was changed
		*    - Sadly, OIS really needs this information, so don't forget to take care of it
		*/
		void updateMouseClippingArea();

		/**
		*  @brief
		*    Inject a mouse pressed event into the input system
		*
		*  @param[in] x
		*    Mouse x position relative to the current focus window
		*  @param[in] y
		*    Mouse y position relative to the current focus window
		*  @param[in] oisMouseButtonId
		*    ID of the pressed OIS mouse button
		*/
		void injectMousePressed(int x, int y, const OIS::MouseButtonID& oisMouseButtonId) const;

		/**
		*  @brief
		*    Inject a mouse released event into the input system
		*
		*  @param[in] x
		*    Mouse x position relative to the current focus window
		*  @param[in] y
		*    Mouse y position relative to the current focus window
		*  @param[in] oisMouseButtonId
		*    ID of the released OIS mouse button
		*/
		void injectMouseReleased(int x, int y, const OIS::MouseButtonID& oisMouseButtonId) const;

		/**
		*  @brief
		*    Update OIS devices (keyboard and mouse)
		*/
		void updateOisDevices();

		/**
		*  @brief
		*    Synchronize OIS feeded devices from internal OIS state
		*/
		void synchronizeOisDevices();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void updateMouseClippingArea(const OIS::MouseState& oisMouseState) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		OIS::InputManager*	mOisInputManager;		///< OIS input manager, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		OIS::Keyboard*		mOisKeyboard;			///< OIS keyboard device, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		OIS::Mouse*			mOisMouse;				///< OIS mouse device, can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		OIS::JoyStick*		mOisJoystick;			///< OIS joystick device, can be a null pointer, we're responsible for destroying the instance in case we no longer need it

		OisKeyboardFeeder*	mOisKeyboardFeeder;
		OisMouseFeeder*		mOisMouseFeeder;
		OisJoystickFeeder*	mOisJoystickFeeder;
		bool				mSynchronized;			///< If "false", the QSF mouse device was not synchronized with OIS after startup yet


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/ois/OisManager-inl.h"
