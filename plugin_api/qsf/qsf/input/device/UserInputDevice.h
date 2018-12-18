// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/device/InputDevice.h"
#include "qsf/input/InputListener.h"

#include <vector>


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
	*    User-defined input device class
	*/
	class QSF_API_EXPORT UserInputDevice : public InputDevice
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		UserInputDevice(const std::string& name, const std::string& description);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~UserInputDevice();

		/**
		*  @brief
		*    Create and insert a new user-defined button
		*/
		InputButton& createButton(const std::string& name, const std::string& description);

		/**
		*  @brief
		*    Create and insert a new user-defined axis
		*/
		InputAxis& createAxis(const std::string& name, const std::string& description);


	//[-------------------------------------------------------]
	//[ Protected qsf::InputDevice methods                    ]
	//[-------------------------------------------------------]
	protected:
		virtual void insertControl(InputControl& control) override;


	//[-------------------------------------------------------]
	//[ Private defintions                                    ]
	//[-------------------------------------------------------]
	private:
		typedef std::vector<InputControl*> InputControlList;	///< List of input controls


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		InputControlList mUserDefinedControls;	///< List of all user-defined input controls; these have to be destroyed in the user input device destructor


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
