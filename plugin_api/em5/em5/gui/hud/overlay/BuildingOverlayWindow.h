// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/hud/overlay/OverlayWindow.h"


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
	*    EMERGENCY 5 building overlay window
	*/
	class EM5_API_EXPORT BuildingOverlayWindow : public OverlayWindow
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit BuildingOverlayWindow(qsf::GuiContext& guiContext);
		explicit BuildingOverlayWindow(qsf::GuiContext& guiContext, Rocket::Core::ElementDocument& rocketCoreElementDocument);
		virtual ~BuildingOverlayWindow();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
