// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/feeder/KeyboardDeviceFeeder.h"

#include <OIS/OISKeyboard.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
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
	*    Input device feeder class to feed the keyboard device from OIS
	*/
	class QSF_API_EXPORT OisKeyboardFeeder : public KeyboardDeviceFeeder, public OIS::KeyListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit OisKeyboardFeeder(KeyboardDevice& device);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OisKeyboardFeeder();

		/**
		*  @brief
		*    Update keyboard device state from OIS internal state
		*
		*  @note
		*    - Don't explicitly call this, except you know what you do
		*/
		void synchronizeWithOis();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputDeviceFeeder methods         ]
	//[-------------------------------------------------------]
	public:
		virtual void updateDeviceFeeder() override;


	//[-------------------------------------------------------]
	//[ Public virtual OIS::KeyListener methods               ]
	//[-------------------------------------------------------]
	public:
		virtual bool keyPressed(const OIS::KeyEvent& oisKeyEvent) override;
		virtual bool keyReleased(const OIS::KeyEvent& oisKeyEvent) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void addBinding(InputButton& button, OIS::KeyCode keyCode);
		InputButton* getButtonByOisKeyCode(OIS::KeyCode keyCode) const;
		bool getButtonState(OIS::Keyboard* oisKeyboard, OIS::KeyCode keyCode) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<OIS::KeyCode, InputButton*> ButtonByOisKeyCode;
		ButtonByOisKeyCode mButtonByOisKeyCode;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
