// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/event/InputButtonEvent.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class KeyboardDevice;
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
	*    Keyboard event class
	*
	*  @remarks
	*    The keyboard event derives from InputButtonEvent, because it only modifies one input button.
	*    Nevertheless, it can't be recognized as InputButtonEvent via getTypeId(), so it has to generate another
	*    InputButtonEvent in onAddToQueue() to correctly inform all listeners that recognize InputButtonEvents only.
	*/
	class QSF_API_EXPORT KeyboardEvent : public InputButtonEvent
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 INPUT_EVENT_ID;	///< "qsf::KeyboardEvent" unique event ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		KeyboardEvent(InputButton& button, bool pressed, uint32 character = 0);

		/**
		*  @brief
		*    Return associated keyboard device
		*/
		KeyboardDevice& getKeyboardDevice() const;

		/**
		*  @brief
		*    Return type ID, which is the string hash of the event class name
		*/
		virtual uint32 getTypeId() const override;

		/**
		*  @brief
		*    Called right before event is inserted into the input system's event queue
		*/
		virtual void onAddToQueue() const override;

		/**
		*  @brief
		*    Apply event to the associated device by changing its state
		*/
		virtual void applyToDevice() const override;


	//[-------------------------------------------------------]
	//[ Public data                                           ]
	//[-------------------------------------------------------]
	public:
		uint32 mCharacter;		///< Unicode character of the key


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
