// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/audio/AudioProxy.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/job/JobProxy.h>

#include <glm/glm.hpp>

#include <boost/container/flat_map.hpp>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class MiniMapRenderer;
}
namespace qsf
{
	class Map;
	class JobArguments;
}
namespace Rocket
{
	namespace Core
	{
		class Geometry;
		class ElementDocument;
		template<typename T> class StringBase;
		typedef StringBase<char> String;
	}
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
	*    EMERGENCY 5 mini-map window HUD element
	*/
	class EM5_API_EXPORT MiniMapWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MiniMapWindow(qsf::GuiContext& guiContext);
		virtual ~MiniMapWindow();

		void setMap(qsf::Map& map);
		void rebuildGui(bool forceRebuild = false);
		inline bool isUnitOverviewButtonVisible() const;
		void setUnitOverviewButtonVisible(bool visible);
		void setPingButtonVisible(bool visible);
		void setFilterButtonsVisible(bool visible);

		inline MiniMapRenderer& getRenderer() const;
		inline Rocket::Core::Geometry* getLibRocketGeometry() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupUI();
		void onReloadGui();

		void update(const qsf::JobArguments& jobArguments);

		void updateFilterButtons();
		void updateFilterButton(Rocket::Core::ElementDocument& elementDocument, const Rocket::Core::String& id, uint32 miniMapIconBucket);
		void updateZoom();
		void updateUnitOverviewButtonVisibleState();
		void updatePingButtonVisibleState();
		void updatePingButton();

		void onLeftMouseButton(Rocket::Core::Event& event, bool isDown);
		void onRightMouseButton(Rocket::Core::Event& event);
		void onMouseMove();
		glm::vec2 mapMousePositionToViewportPosition(Rocket::Core::Event& event);
		glm::vec3 getWorldPositionFromViewportPosition(Rocket::Core::Event& event);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MiniMapRenderer*		mMiniMapRenderer;		///< Mini map renderer instance (valid as of construction)
		Rocket::Core::Geometry* mMapGeometry;			///< Generated as soon as a map is set, can be a null pointer
		bool					mMapGeometryBuilt;
		bool					mWaitingForPingClick;
		bool					mUnitOverviewButtonVisible;
		bool					mPingButtonVisible;
		bool					mFilterButtonsVisible;
		qsf::JobProxy			mAnimationJobProxy;		///< Regular animation update job proxy; for updates once a frame
		AudioProxy				mAudioProxy;

		// Needed for feature to move the minimap viewport around via left mouse down and drag
		glm::vec2				mCurrentMousePositionOnViewport;
		glm::vec2				mMousePositionOnViewportOnMouseDown;
		glm::vec2				mLastMousePositionOnDrag;
		bool					mLeftMouseDown;

		// TODO(sw) This is an fix for the crash when going from the game to the main menu and initiate then a gui hot reloading -> The connection to the minimap was still there but the minimap instance was destroyed
		// Proper fix see QSF Bug "H301 GUI | "qsf::GuiDocument" | We need to guarantee that for each e.g. "onPostShow()" there's a "onPostHide()""
		boost::signals2::connection	mReloadGuiConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/hud/MiniMapWindow-inl.h"
