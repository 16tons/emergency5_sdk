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
	class BuildingOverlayWindow;
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
	*    EMERGENCY 5 building overlay window manager
	*/
	class BuildingOverlayWindowManager : public OverlayWindowManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit BuildingOverlayWindowManager(qsf::GuiContext& guiContext);
		virtual ~BuildingOverlayWindowManager();


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
		typedef std::unordered_map<uint64, BuildingOverlayWindow*> BuildingOverlayWindowMap;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onReloadGui();
		void createElementDocument();
		void destroyElementDocument();
		void destroyAllWindows();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool								mAllOverlaysVisible;
		Rocket::Core::ElementDocument*		mElementDocument;				///< Holds the libRocket document which will be clone for new building overlay window elements
		BuildingOverlayWindowMap			mBuildingOverlayWindowMap;		///< Holds a list of currently used building overlay window instance
		std::vector<BuildingOverlayWindow*>	mFreeBuildingOverlayWindowList;	///< Holds a list of currently not needed building overlay window instances for reusage


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
