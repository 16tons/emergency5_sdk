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
	*    Input axis control
	*/
	class QSF_API_EXPORT InputAxis : public InputControl
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 INPUT_CONTROL_ID;	///< "qsf::InputAxis" unique control ID


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
		*  @param[in] sName
		*    Control name
		*  @param[in] description
		*    Control description
		*/
		inline InputAxis(InputDevice& device, const std::string& name, const std::string& description);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~InputAxis();

		/**
		*  @brief
		*    Get axis value
		*
		*  @return
		*    Current value
		*
		*  @remarks
		*    Please note that a value can be absolute (for instance the x-axis of a joystick) or
		*    relative (for instance the x-axis of a mouse). While an absolute axis is usually timing
		*    independent, a relative axis just tells you about a state change since the last update.
		*/
		inline float getValue() const;

		/**
		*  @brief
		*    Set axis value
		*
		*  @param[in] value
		*    Current value
		*/
		inline void setValue(float value);

		/**
		*  @brief
		*    Get axis value
		*
		*  @see
		*    - getValue()
		*/
		inline float operator()() const;


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
		float mValue;	///< Value of the axis


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/control/InputAxis-inl.h"
