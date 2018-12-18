// Copyright (C) 2012-2018 Promotion Software GmbH


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
	*    EMERGENCY 5 status overlay window
	*/
	class EM5_API_EXPORT StatusOverlayWindow : public OverlayWindow
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit StatusOverlayWindow(qsf::GuiContext& guiContext);
		explicit StatusOverlayWindow(qsf::GuiContext& guiContext, Rocket::Core::ElementDocument& rocketCoreElementDocument);
		virtual ~StatusOverlayWindow();


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
