// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/control/InputControl.h" // InputDevice-inl.h needs this

#include <boost/container/flat_map.hpp>

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class InputDeviceManager;
	class InputButton;
	class InputAxis;
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
	*    Input device base class
	*/
	class QSF_API_EXPORT InputDevice
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
	friend class InputDeviceManager;	// For access to mRegisteredAtManager
	friend class InputControl;			// For addControl()


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] name
		*    Control name
		*  @param[in] description
		*    Control description
		*/
		inline InputDevice(const std::string& name, const std::string& description);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~InputDevice();

		/**
		*  @brief
		*    Get control name
		*
		*  @return
		*    Name
		*/
		inline const std::string& getName() const;

		/**
		*  @brief
		*    Get control description
		*
		*  @return
		*    Description
		*/
		inline const std::string& getDescription() const;

		/**
		*  @brief
		*    Get list of all controls of the device
		*/
		inline const std::vector<InputControl*>& getControls() const;

		/**
		*  @brief
		*    Get list of all buttons
		*/
		inline const std::vector<InputButton*>& getButtons() const;

		/**
		*  @brief
		*    Get list of all axes
		*/
		inline const std::vector<InputAxis*>& getAxes() const;

		/**
		*  @brief
		*    Get input control with a specific name
		*
		*  @param[in] name
		*    Name of control
		*
		*  @return
		*    Control, or a null pointer if no input control with that name could be found
		*/
		InputControl* getControlByName(const StringHash& name) const;

		/**
		*  @brief
		*    Get input control with a specific name
		*
		*  @param[in] name
		*    Name of control
		*
		*  @return
		*    Control, or a null pointer if no input control with that name could be found
		*/
		template<typename TYPE>
		inline TYPE* getByName(const StringHash& name) const;

		/**
		*  @brief
		*    Reset all controls of the device
		*/
		void resetDevice();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputDevice methods               ]
	//[-------------------------------------------------------]
	public:
		inline virtual void updateDevice();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InputDevice methods            ]
	//[-------------------------------------------------------]
	protected:
		virtual void insertControl(InputControl& control);


	//[-------------------------------------------------------]
	//[ Protected definitions                                 ]
	//[-------------------------------------------------------]
	protected:
		typedef boost::container::flat_map<uint32, InputControl*> ControlsByIdMap;


	//[-------------------------------------------------------]
	//[ Protected data                                        ]
	//[-------------------------------------------------------]
	protected:
		std::string				 mName;					///< Device name
		std::string				 mDescription;			///< Device description
		InputDeviceManager*		 mRegisteredAtManager;	///< The input device manager this device is registered at, or null pointer if there is none

		ControlsByIdMap				mControlsById;		///< Map of input controls by their IDs
		std::vector<InputControl*>	mControls;			///< List of all controls
		std::vector<InputButton*>	mButtons;			///< List of buttons
		std::vector<InputAxis*>		mAxes;				///< List of axes


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/input/device/InputDevice-inl.h"
