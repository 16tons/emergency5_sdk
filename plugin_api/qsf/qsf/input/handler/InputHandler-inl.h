// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InputHandler::InputHandler(uint32 id) :
		mId(id)
	{
		// Nothing here
	}

	inline InputHandler::~InputHandler()
	{
		// Nothing here
	}

	inline uint32 InputHandler::getId()
	{
		// Return input handler ID
		return mId;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputHandler methods              ]
	//[-------------------------------------------------------]
	inline void InputHandler::initialize()
	{
		// Nothing here
	}

	inline void InputHandler::updateInputHandler(const Time& timePassed)
	{
		// Nothing here
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	inline void InputHandler::eventReceived(const InputEvent& inputEvent, Response& response)
	{
		// Nothing here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
