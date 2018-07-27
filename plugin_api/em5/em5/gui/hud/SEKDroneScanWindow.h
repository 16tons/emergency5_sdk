// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/job/JobProxy.h>
#include <qsf/map/Entity.h>

#include <glm/glm.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class HeatRenderer;
}
namespace qsf
{
	class Map;
}
namespace Rocket
{
	namespace Core
	{
		class Geometry;
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
	*    EMERGENCY 5 SEK drone scan HUD element
	*/
	class EM5_API_EXPORT SEKDroneScanWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit SEKDroneScanWindow(qsf::GuiContext& guiContext);
		virtual ~SEKDroneScanWindow();

		void rebuildGui();
		void setMap(qsf::Map& map);

		void setCameraDrone(qsf::Entity& entityWithCamera);

		inline HeatRenderer& getHeatRenderer();
		inline Rocket::Core::Geometry& getGeometry();

		void update(const qsf::JobArguments& jobArguments);


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
		virtual void onPostHide() override;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		HeatRenderer*			  mHeatRenderer;				///< Heat renderer instance  (valid as of construction), we have to delete this instance
		Rocket::Core::Geometry*	  mViewportGeometry;			///< Generated as soon as a map is set, we have to delete this instance
		qsf::WeakPtr<qsf::Entity> mDroneEntity;					///< The camera is on this (gameplay) entity, don't delete this
		glm::vec2				  mViewportSize;
		bool					  mViewportGeometryBuilt;
		bool					  mHeatRendererInitialized;
		qsf::JobProxy			  mAnimationJobProxy;			///< Regular animation update job proxy; for updates once a frame


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/gui/hud/SEKDroneScanWindow-inl.h"
