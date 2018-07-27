// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline WindowApplication::~WindowApplication()
	{
		// Nothing to do in here
	}

	inline RenderWindow* WindowApplication::getRenderWindow() const
	{
		return mRenderWindow;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline WindowApplication::WindowApplication() :
		TimedApplication(),
		mRenderWindow(nullptr)
	{
		// Nothing to do in here
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
