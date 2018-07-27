// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/hud/overlay/OverlayWindowManager.h"

#include <qsf/gui/GuiDocument.h>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class StatusOverlayWindow;
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
	*    EMERGENCY 5 status overlay window manager
	*/
	class StatusOverlayWindowManager : public OverlayWindowManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit StatusOverlayWindowManager(qsf::GuiContext& guiContext);
		virtual ~StatusOverlayWindowManager();


	//[-------------------------------------------------------]
	//[ Public virtual em5::OverlayWindowManager methods      ]
	//[-------------------------------------------------------]
	public:
		virtual void setAllOverlaysVisible(bool visible) override;
		virtual OverlayWindow* getOverlayWindowInstance(uint64 entityId) override;
		virtual void releaseOverlayWindowInstance(uint64 entityId) override;


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef std::unordered_map<uint64, StatusOverlayWindow*> StatusOverlayWindowMap;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool								mAllOverlaysVisible;
		Rocket::Core::ElementDocument*		mElementDocument;				///< Holds the libRocket document which will be clone for new status overlay window elements
		StatusOverlayWindowMap				mStatusOverlayWindowMap;		///< Holds a list of currently used status overlay window instance
		std::vector<StatusOverlayWindow*>	mFreeStatusOverlayWindowList;	///< Holds a list of currently not needed status overlay window instances for reusage


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
