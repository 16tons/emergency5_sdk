// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/InputListener.h"
#include "qsf/job/JobProxy.h"
#include "qsf/input/event/keyboard/KeyboardEvent.h"

#include <Rocket/Core/Input.h>

#include <boost/noncopyable.hpp>
#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class Context;
		class Event;
	}
}
namespace qsf
{
	class KeyboardEvent;
	class InputButton;
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
	*    GUI context input listener class connecting QSF input and libRocket
	*
	*  @note
	*    - Based on the libRocket SDK OGRE example written by Peter Curry
	*    - The GUI context input listener adds itself automatically to the input system
	*    - The GUI context input listener removes itself automatically from the input system
	*/
	class GuiContextInputListener : public InputListener, public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Return the key modifier state
		*
		*  @return
		*    The key modifier state
		*/
		static int getKeyModifierState();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] rocketCoreContext
		*    libRocket context to feed with input, must stay alive as long as this input listener instance exists
		*/
		explicit GuiContextInputListener(Rocket::Core::Context& rocketCoreContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GuiContextInputListener();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Build key maps
		*/
		void buildKeyMaps();

		/**
		*  @brief
		*    Map an input button to a libRocket key identifier
		*/
		Rocket::Core::Input::KeyIdentifier getKeyIdentifierByButton(const InputButton& inputButton) const;

		/**
		*  @brief
		*    Process a key pressed event
		*/
		void keyPressed(const KeyboardEvent& keyboardEvent);

		/**
		*  @brief
		*    Process a key released event
		*/
		void keyReleased(const KeyboardEvent& keyboardEvent);

		/**
		*  @brief
		*    Process a libRocket event
		*/
		void reactToRocketEvent(Rocket::Core::Event& event);

		/**
		*  @brief
		*    Updates the key hold event
		*/
		void onKeyHold(const JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<uint32, Rocket::Core::Input::KeyIdentifier> KeyIdentifierMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		Rocket::Core::Context*				mRocketCoreContext;			///< libRocket context to feed with input, always valid, don't forget to remove the reference in case we no longer need it
		KeyIdentifierMap					mKeyIdentifiers;			///< QSF input button ID to "Rocket::Core::Input::KeyIdentifier" map
		Response*							mCurrentResponse;			///< Current event listener response; only valid inside a "eventReceived" call
		JobProxy							mHoldKeyJobProxy;			///< Update hold key proxy
		bool								mIsKeyPressed;				///< This tell us weather a key is hold pressed.
		float								mKeyHoldTime;				///< Holds the time a key was pressed;
		Rocket::Core::Input::KeyIdentifier	mLastTextInputKeyIdentifier;///< Hold the last pressed key identifier.
		uint32								mLastCharacter;				///< Holds the last pressed character


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
