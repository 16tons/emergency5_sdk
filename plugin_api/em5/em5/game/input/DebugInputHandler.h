// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/input/handler/InputHandler.h>
#include <qsf/message/MessageProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class KeyboardEvent;
	class MouseButtonEvent;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    EMERGENCY 5 debug input handler class
	*/
	class DebugInputHandler : public qsf::InputHandler
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit DebugInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DebugInputHandler();


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InputHandler methods           ]
	//[-------------------------------------------------------]
	protected:
		virtual void updateInputHandler(const qsf::Time& timePassed) override;


	//[-------------------------------------------------------]
	//[ Protected virtual qsf::InputListener methods          ]
	//[-------------------------------------------------------]
	protected:
		virtual void eventReceived(const qsf::InputEvent& inputEvent, Response& response) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void processKeyboardEvent(const qsf::KeyboardEvent& keyboardEvent);
		void processMouseButtonEvent(const qsf::MouseButtonEvent& mouseButtonEvent);

		void clearFreeplayEvents();

		// Camera switch
		void switchToCameraView(int number);
		void onScreenFadedOutForCameraView(const qsf::MessageParameters& parameters, int number);
		void onCutsceneFinishedForCameraView(const qsf::MessageParameters& parameters, int number);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32			  mCurrentWeatherStateIndex;
		qsf::MessageProxy mMessageProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
