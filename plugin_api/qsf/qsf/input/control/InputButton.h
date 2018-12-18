// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/control/InputControl.h"


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
	*    Input button control
	*/
	class QSF_API_EXPORT InputButton : public InputControl
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 INPUT_CONTROL_ID;	///< "qsf::InputButton" unique control ID


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] device
		*    Owning device
		*  @param[in] name
		*    Control name
		*  @param[in] description
		*    Control description
		*  @param[in] index
		*    Index associated with the button, 0 if none
		*/
		inline InputButton(InputDevice& device, const std::string& name, const std::string& description, uint32 index = 0);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InputButton();

		/**
		*  @brief
		*    Get index
		*
		*  @return
		*    Index of the button
		*/
		inline uint32 getIndex() const;

		/**
		*  @brief
		*    Get button status
		*
		*  @return
		*    "true", if the button is currently pressed, else "false"
		*/
		inline bool isPressed() const;

		/**
		*  @brief
		*    Set button status
		*
		*  @param[in] bPressed
		*    "true", if the button is pressed, else "false"
		*/
		inline void setPressed(bool bPressed);

		/**
		*  @brief
		*    Check if the button has been hit
		*
		*  @return
		*    "true", if the button has been hit since the last call of this function, else "false"
		*
		*  @remarks
		*    This will return the hit-state of the button and also reset it immediately (so the next call
		*    to IsHit() will return false). If you only want to check, but not reset the hit-state of
		*    a button, you should call CheckHit().
		*/
		inline bool isHit();

		/**
		*  @brief
		*    Check if the button has been hit
		*
		*  @return
		*    "true", if the button has been hit since the last call of this function, else "false"
		*
		*  @remarks
		*    This function will NOT reset the hit-state after being called (see IsHit()).
		*/
		inline bool checkHit();

		/**
		*  @brief
		*    Get button status
		*
		*  @see
		*    - getPressed()
		*/
		inline bool operator()() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputControl methods              ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Reset state of this control back to default
		*/
		inline virtual void reset() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Button info
		uint32 mIndex;		///< Index of the button, default is 0

		// Button status
		bool mPressed;		///< Is the button currently pressed?
		bool mHit;			///< Has the button been hit in the meantime?


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/control/InputButton-inl.h"
