// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline Map& DebugDrawManager::getMap() const
	{
		return mMap;
	}

	inline GuiContext* DebugDrawManager::getGuiContext() const
	{
		return mGuiContext;
	}

	inline void DebugDrawManager::setGuiContext(GuiContext* guiContext)
	{
		if (nullptr == guiContext)
		{
			cancelAllRequests();
		}
		mGuiContext = guiContext;
	}


	//[-------------------------------------------------------]
	//[ Protected methods                                     ]
	//[-------------------------------------------------------]
	inline DebugDrawManager::DebugDrawManager(Map& map) :
		mMap(map),
		mGuiContext(nullptr),
		mRocketElementDocument(nullptr),
		mDebugDrawsVisible(true),
		mNextId(0)
	{
		// Nothing to do in here
	}

	inline DebugDrawManager::~DebugDrawManager()
	{
		cancelAllRequests();
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
