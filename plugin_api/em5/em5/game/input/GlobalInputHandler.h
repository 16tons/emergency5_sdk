// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/input/handler/InputHandler.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class KeyboardEvent;
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
	*    EMERGENCY 5 global input handler class
	*/
	class EM5_API_EXPORT GlobalInputHandler : public qsf::InputHandler
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit GlobalInputHandler(uint32 id);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GlobalInputHandler();

		/**
		*  @brief
		*    Update method that gets regularly called
		*/
		void update();


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


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
