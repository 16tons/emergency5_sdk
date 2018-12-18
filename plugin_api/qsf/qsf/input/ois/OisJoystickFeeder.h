// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/feeder/JoystickDeviceFeeder.h"

#include <OIS/OISJoyStick.h>

#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class InputAxis;
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
	*    Input device feeder class to feed the joystick device from OIS
	*/
	class QSF_API_EXPORT OisJoystickFeeder : public JoystickDeviceFeeder, public OIS::JoyStickListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit OisJoystickFeeder(JoystickDevice& device);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OisJoystickFeeder();

		/**
		*  @brief
		*    Helper method to get the joystick device's axis by the given OIS joystick axis index
		*/
		InputAxis* getAxisByOisJoystickAxisIndex(int oisJoystickAxisIndex) const;

		/**
		*  @brief
		*    Helper method to get the joystick device's button by the given OIS joystick button ID
		*/
		InputButton* getButtonByOisJoystickButtonId(int oisJoystickButtonId) const;

		/**
		*  @brief
		*    Update joystick device state from OIS internal state
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
	//[ Public virtual OIS::JoyStickListener methods          ]
	//[-------------------------------------------------------]
	public:
		virtual bool buttonPressed(const OIS::JoyStickEvent& oisJoystickEvent, int oisJoystickButtonId) override;
		virtual bool buttonReleased(const OIS::JoyStickEvent& oisJoystickEvent, int oisJoystickButtonId) override;
		virtual bool axisMoved(const OIS::JoyStickEvent& oisJoystickEvent, int axis) override;
		virtual bool sliderMoved(const OIS::JoyStickEvent& oisJoystickEvent, int index) override;
		virtual bool povMoved(const OIS::JoyStickEvent& oisJoystickEvent, int index) override;
		virtual bool vector3Moved(const OIS::JoyStickEvent& oisJoystickEvent, int index) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void addBinding(InputAxis& axis, int oisJoystickAxisId);
		void addBinding(InputButton& button, int oisJoystickButtonId);
		bool getButtonState(OIS::JoyStick* oisJoystick, int oisJoystickButtonId) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<int, InputAxis*> AxisByOisJoystickAxisIndex;
		typedef boost::container::flat_map<int, InputButton*> ButtonByOisJoystickButtonId;

		AxisByOisJoystickAxisIndex mAxisByOisJoystickAxisIndex;
		ButtonByOisJoystickButtonId mButtonByOisJoystickButtonId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
