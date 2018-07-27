// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/input/feeder/MouseDeviceFeeder.h"

#include <OIS/OISMouse.h>

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
	*    Input device feeder class to feed the mouse device from OIS
	*/
	class QSF_API_EXPORT OisMouseFeeder : public MouseDeviceFeeder, public OIS::MouseListener
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit OisMouseFeeder(MouseDevice& device);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~OisMouseFeeder();

		/**
		*  @brief
		*    Helper method to get the mouse device's button with the given OIS mouse button ID
		*/
		InputButton* getButtonByOisMouseButtonId(OIS::MouseButtonID oisMouseButtonId) const;

		/**
		*  @brief
		*    Update mouse device state from OIS internal state
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
	//[ Public virtual OIS::MouseListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual bool mouseMoved(const OIS::MouseEvent& oisMouseEvent) override;
		virtual bool mousePressed(const OIS::MouseEvent& oisMouseEvent, OIS::MouseButtonID oisMouseButtonId) override;
		virtual bool mouseReleased(const OIS::MouseEvent& oisMouseEvent, OIS::MouseButtonID oisMouseButtonId) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void addBinding(InputButton& button, OIS::MouseButtonID oisMouseButtonId);
		bool getButtonState(OIS::Mouse* oisMouse, OIS::MouseButtonID oisMouseButtonId) const;

		glm::vec2 getMousePosition(const OIS::MouseState& oisMouseState) const;
		glm::vec2 getMouseMovement(const OIS::MouseState& oisMouseState) const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<OIS::MouseButtonID, InputButton*> ButtonByOisMouseButtonId;
		ButtonByOisMouseButtonId mButtonByOisMouseButtonId;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
