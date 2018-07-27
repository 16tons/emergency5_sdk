// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool MiniMapWindow::isUnitOverviewButtonVisible() const
	{
		return mUnitOverviewButtonVisible;
	}

	inline MiniMapRenderer& MiniMapWindow::getRenderer() const
	{
		return *mMiniMapRenderer;
	}

	inline Rocket::Core::Geometry* MiniMapWindow::getLibRocketGeometry() const
	{
		return mMapGeometry;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
