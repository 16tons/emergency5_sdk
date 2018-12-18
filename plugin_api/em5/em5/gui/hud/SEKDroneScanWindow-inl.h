// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline HeatRenderer& SEKDroneScanWindow::getHeatRenderer()
	{
		return *mHeatRenderer;
	}

	inline Rocket::Core::Geometry& SEKDroneScanWindow::getGeometry()
	{
		return *mViewportGeometry;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
