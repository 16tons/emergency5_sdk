// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline InputListener::InputListener() :
		mFocusWindow(nullptr)
	{
		// Nothing to do in here
	}

	inline InputListener::~InputListener()
	{
		// Nothing to do in here
	}

	inline Window* InputListener::getFocusWindow() const
	{
		return mFocusWindow;
	}

	inline void InputListener::setFocusWindow(Window* focusWindow)
	{
		mFocusWindow = focusWindow;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	inline void InputListener::eventReceived(const InputEvent& inputEvent, Response& response)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
