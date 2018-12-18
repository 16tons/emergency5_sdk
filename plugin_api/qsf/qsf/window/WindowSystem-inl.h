// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline WindowSystem::WindowSystem() :
		mNativeWindowHandle(NULL_HANDLE),
		mIsNativeWindowHandleAWindow(false)
	{
		// Nothing to do in here
	}

	inline WindowSystem::~WindowSystem()
	{
		// Nothing to do in here
	}

	inline const std::vector<Window*>& WindowSystem::getWindows() const
	{
		return mWindows;
	}

	inline handle WindowSystem::getFocusNativeWindowHandle() const
	{
		return mNativeWindowHandle;
	}

	inline bool WindowSystem::isFocusNativeWindowHandleAWindow() const
	{
		return mIsNativeWindowHandleAWindow;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* WindowSystem::getName() const
	{
		return "Window system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
