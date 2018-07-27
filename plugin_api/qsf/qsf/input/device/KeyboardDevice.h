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
	*    Keyboard input device class
	*/
	class QSF_API_EXPORT KeyboardDevice : public InputDevice
	{


	//[-------------------------------------------------------]
	//[ Device definition                                     ]
	//[-------------------------------------------------------]
	public:
		InputButton LeftShift;			///< Left shift key
		InputButton RightShift;			///< Right shift key
		InputButton LeftControl;		///< Left control key ("Ctrl")
		InputButton RightControl;		///< Right control key ("Ctrl")
		InputButton LeftMenu;			///< Left menu key ("Alt")
		InputButton RightMenu;			///< Right menu key ("Alt Gr" in german keyboard)
		InputButton CapsLock;			///< Caps lock
		InputButton NumLock;			///< Num lock
		InputButton ScrollLock;			///< Scroll lock
		InputButton Backspace;			///< Backspace
		InputButton Tab;				///< Tab
		InputButton Clear;				///< Clear (not available everywhere)
		InputButton Return;				///< Return (often the same as "Enter")
		InputButton Pause;				///< Pause
		InputButton Escape;				///< Escape
		InputButton Space;				///< Space
		InputButton Home;				///< Home
		InputButton End;				///< End
		InputButton Up;					///< Up arrow
		InputButton Down;				///< Down arrow
		InputButton Left;				///< Left arrow
		InputButton Right;				///< Right arrow
		InputButton PageUp;				///< Page up
		InputButton PageDown;			///< Page down
		InputButton Insert;				///< Insert
		InputButton Delete;				///< Delete
		InputButton Select;				///< Select (not available everywhere)
		InputButton Execute;			///< Execute (not available everywhere)
		InputButton Print;				///< Print screen
		InputButton Help;				///< Help (not available everywhere)
		InputButton Key0;				///< 0 (control name is "0")
		InputButton Key1;				///< 1 (control name is "1")
		InputButton Key2;				///< 2 (control name is "2")
		InputButton Key3;				///< 3 (control name is "3")
		InputButton Key4;				///< 4 (control name is "4")
		InputButton Key5;				///< 5 (control name is "5")
		InputButton Key6;				///< 6 (control name is "6")
		InputButton Key7;				///< 7 (control name is "7")
		InputButton Key8;				///< 8 (control name is "8")
		InputButton Key9;				///< 9 (control name is "9")
		InputButton A;					///< A
		InputButton B;					///< B
		InputButton C;					///< C
		InputButton D;					///< D
		InputButton E;					///< E
		InputButton F;					///< F
		InputButton G;					///< G
		InputButton H;					///< H
		InputButton I;					///< I
		InputButton J;					///< J
		InputButton K;					///< K
		InputButton L;					///< L
		InputButton M;					///< M
		InputButton N;					///< N
		InputButton O;					///< O
		InputButton P;					///< P
		InputButton Q;					///< Q
		InputButton R;					///< R
		InputButton S;					///< S
		InputButton T;					///< T
		InputButton U;					///< U
		InputButton V;					///< V
		InputButton W;					///< W
		InputButton X;					///< X
		InputButton Y;					///< Y
		InputButton Z;					///< Z
		InputButton Numpad0;			///< Numeric keypad 0
		InputButton Numpad1;			///< Numeric keypad 1
		InputButton Numpad2;			///< Numeric keypad 2
		InputButton Numpad3;			///< Numeric keypad 3
		InputButton Numpad4;			///< Numeric keypad 4
		InputButton Numpad5;			///< Numeric keypad 5
		InputButton Numpad6;			///< Numeric keypad 6
		InputButton Numpad7;			///< Numeric keypad 7
		InputButton Numpad8;			///< Numeric keypad 8
		InputButton Numpad9;			///< Numeric keypad 9
		InputButton NumpadPlus;			///< Numeric keypad "Plus"
		InputButton NumpadMinus;		///< Numeric keypad "Minus"
		InputButton NumpadMultiply;		///< Numeric keypad "Multiply"
		InputButton NumpadDivide;		///< Numeric keypad "Divide"
		InputButton NumpadSeparator;	///< Numeric keypad "Separator"
		InputButton NumpadDecimal;		///< Numeric keypad "Decimal"
		InputButton NumpadEnter;		///< Numeric keypad "Enter"
		InputButton F1;					///< F1
		InputButton F2;					///< F2
		InputButton F3;					///< F3
		InputButton F4;					///< F4
		InputButton F5;					///< F5
		InputButton F6;					///< F6
		InputButton F7;					///< F7
		InputButton F8;					///< F8
		InputButton F9;					///< F9
		InputButton F10;				///< F10
		InputButton F11;				///< F11
		InputButton F12;				///< F12
		InputButton F13;				///< F13 key
		InputButton F14;				///< F14 key
		InputButton F15;				///< F15 key
		InputButton F16;				///< F16 key
		InputButton F17;				///< F17 key
		InputButton F18;				///< F18 key
		InputButton F19;				///< F19 key
		InputButton F20;				///< F20 key
		InputButton F21;				///< F21 key
		InputButton F22;				///< F22 key
		InputButton F23;				///< F23 key
		InputButton F24;				///< F24 key
		InputButton Circumflex;			///< Circumflex (^)
		InputButton LeftWindows;		///< Left Windows key (natural keyboard)
		InputButton RightWindows;		///< Right Windows key (natural keyboard)
		InputButton Applications;		///< Applications key (natural keyboard)
		InputButton VolumeMute;			///< Volume mute key
		InputButton VolumeDown;			///< Volume down key
		InputButton VolumeUp;			///< Volume up key
		InputButton MediaNextTrack;		///< Next track key
		InputButton MediaPreviousTrack;	///< Previous track key
		InputButton MediaStop;			///< Stop media key
		InputButton MediaPlayPause;		///< Play/pause media key
		InputButton Plus;				///< For any country/region, the '+' key
		InputButton Minus;				///< For any country/region, the '-' key
		InputButton Comma;				///< For any country/region, the ',' key
		InputButton Period;				///< For any country/region, the '.' key
		InputButton OEM1;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ';:' key.
		InputButton OEM2;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '/?' key.
		InputButton OEM3;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '`~' key.
		InputButton OEM4;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '[{' key.
		InputButton OEM5;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the '\|' key.
		InputButton OEM6;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the ']}' key.
		InputButton OEM7;				///< Used for miscellaneous characters; it can vary by keyboard. For the US standard keyboard, the 'single-quote/double-quote' key.
		InputButton OEM8;				///< Used for miscellaneous characters; it can vary by keyboard.
		InputButton OEM102;				///< Either the angle bracket key (<>|) or the backslash key on the RT 102-key keyboard


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		KeyboardDevice();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~KeyboardDevice();

		/**
		*  @brief
		*    Return true if any Shift key is currently pressed.
		*/
		bool anyShiftPressed() const;

		/**
		*  @brief
		*    Return true if any Control ("Ctrl") key is currently pressed.
		*/
		bool anyControlPressed() const;

		/**
		*  @brief
		*    Return true if any Menu ("Alt") key is currently pressed.
		*/
		bool anyMenuPressed() const;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
