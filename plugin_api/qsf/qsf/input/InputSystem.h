// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/base/System.h"
#include "qsf/input/InputDeviceManager.h"
#include "qsf/job/JobProxy.h"

#include <boost/container/flat_map.hpp>

#include <vector>
#include <deque>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class OisManager;
	class InputListener;
	class InputEvent;
	class InputHandler;
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
	*    Input system class
	*
	*  @note
	*    - The external library "Object Oriented Input System" (OIS) ( http://sourceforge.net/projects/wgois/ ) is used
	*
	*  @todo
	*    - TODO(fw): It would be nice to inject arbitrary input events; but that's tricky because of OIS, we would have to change its internal state as well.
	*/
	class QSF_API_EXPORT InputSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		inline InputSystem();

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InputSystem();

		/**
		*  @brief
		*    Return the input device manager
		*/
		inline InputDeviceManager& getInputDeviceManager() const;

		/**
		*  @brief
		*    Add an input listener
		*
		*  @param[in] inputListener
		*    Input listener to add, must stay valid as long as it's registered within the input system
		*
		*  @return
		*    "true" if all went fine, else "false" (input listener already registered?)
		*/
		bool addInputListener(InputListener& inputListener);

		/**
		*  @brief
		*    Remove an input listener
		*
		*  @param[in] inputListener
		*    Input listener to remove
		*
		*  @return
		*    "true" if all went fine, else "false" (input listener not registered?)
		*/
		bool removeInputListener(InputListener& inputListener);

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
		*    Return the OIS manager instance
		*
		*  @return
		*    A pointer to the OIS manager instance, or nullptr if the OIS manager is disabled
		*/
		inline OisManager* getOisManager() const;

		/**
		*  @brief
		*    Return the OIS manager instance
		*
		*  @param[in] enable
		*    "true" to enable the OIS manager, "false" to disable it
		*
		*  @note
		*    - Calling this method invalidates all pointers, that may have been acquired by calling the getOisManager() instance
		*/
		void enableOisManager(bool enable);

		/**
		*  @brief
		*    Add an input event to the event queue
		*
		*  @param[in] inputEvent
		*    Pointer to the event to add; may be a null pointer, but then nothing is done
		*/
		void addEventToInputQueue(InputEvent* inputEvent);

		/**
		*  @brief
		*    Create and add a new input handler
		*
		*  @param[in] id
		*    ID of the input handler, can be used to identify the handler later on
		*
		*  @note
		*    If the ID is already in use by another input handler, this one will be automatically removed
		*/
		template <typename TYPE>
		TYPE& createInputHandler(const StringHash& id);

		/**
		*  @brief
		*    Destroy an input handler
		*
		*  @param[in] id
		*    ID of the input handler to be removed
		*
		*  @return
		*    "true" if a handler with the given ID was found and could be removed, "false" if the ID was not in use
		*/
		bool destroyInputHandler(const StringHash& id);

		/**
		*  @brief
		*    Destroy all input handlers
		*/
		void destroyAllInputHandlers();

		/**
		*  @brief
		*    Return an input handler with the given ID
		*
		*  @param[in] id
		*    ID of the input handler to be found
		*
		*  @return
		*    Pointer to the input handler, or null pointer if no one with that ID exists; do not destroy the returned instance
		*/
		InputHandler* getInputHandlerById(const StringHash& id) const;

		//[-------------------------------------------------------]
		//[ Devices                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Return the keyboard device
		*/
		inline KeyboardDevice& getKeyboard() const;

		/**
		*  @brief
		*    Return the mouse device
		*/
		inline MouseDevice& getMouse() const;

		/**
		*  @brief
		*    Return the joystick device
		*/
		inline JoystickDevice& getJoystick() const;

		/**
		*  @brief
		*    Return the default feeder for user-defined input devices
		*/
		inline UserInputDeviceFeeder& getUserInputDeviceFeeder() const;

		//[-------------------------------------------------------]
		//[ Internal                                              ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Main update function that is passed to the job manager
		*
		*  @param[in] jobArguments
		*    Job arguments
		*/
		void updateJob(const JobArguments& jobArguments);


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
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Add an input handler internally
		*
		*  @param[in] inputHandler
		*    The input handler instance to add
		*/
		void addInputHandler(InputHandler& inputHandler);

		/**
		*  @brief
		*    Set the focus native window handle the input system receives the input from
		*
		*  @param[in] nativeWindowHandle
		*    The focus native window handle the input system receives the input from, can be a null pointer (input system is disabled),
		*    the instance must stay valid as long as it's the focus native window handle of the input system
		*  @param[in] exclusive
		*    Exclusive window handling? (do this e.g. when your window is in fullscreen mode)
		*
		*  @note
		*    - Sadly, OIS is not window independent so we need to handle this because we need to be able to use multiple windows
		*    - In case the focus native window handle gets destroyed, the input system is informed about this and updated automatically
		*    - OIS only receives input events in case the OS window the focus native window handle represents has the OS focus
		*    - In a single window application, this method is usually only called once during the application initialization
		*/
		void setFocusNativeWindowHandle(handle nativeWindowHandle, bool exclusive = false, bool isWindow = true);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<InputListener*> InputListenerList;						///< List of input listeners
		typedef std::deque<InputEvent*> InputEventQueue;							///< Queue of input events
		typedef boost::container::flat_map<uint32, InputHandler*> InputHandlerMap;	///< Map of input handlers, using their ID as key


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		handle						mNativeWindowHandle;	///< The focus native window handle the input system receives the input from, can be a null pointer, do not destroy the instance
		InputDeviceManager*			mInputDeviceManager;	///< Input device and device feeder manager
		OisManager*					mOisManager;			///< OIS manager instance that handles everything OIS-related; can be a null pointer, we're responsible for destroying the instance in case we no longer need it
		JobProxy					mUpdateJobProxy;		///< Job proxy for regular updates

		InputListenerList			mInputListeners;		///< List of input listeners
		InputEventQueue				mInputEventQueue;		///< Input event queue
		InputHandlerMap				mInputHandlerMap;		///< Map of input handlers


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/InputSystem-inl.h"
