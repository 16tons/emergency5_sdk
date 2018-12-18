// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Global definitions                                    ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Render queue group ID
	*
	*  @remarks
	*    Render queues (<first ID>-<last render queue ID that is included in the render>)
	*    - ID 5-9 (shadows disabled): "qsf::RENDER_QUEUE_SKIES_EARLY":
	*    - ID 0-50
	*    - ID 51-51 ("Projected Early")
	*      - Street/decal projection
	*    - ID 52-76 ("Nonprojected geometry pass #2")
	*    - ID 77
	*      - Shine-through
	*    - ID 79
	*      - Liquids
	*    - ID 80-89 (shadows disabled): "qsf::RENDER_QUEUE_8": Final transparent stuff
	*      - Particles (Particle Universe)
	*      - Flares
	*        - Occlusion query
	*        - Visual: +1
	*    - ID 90-94 (shadows disabled): "qsf::RENDER_QUEUE_9": Used for debugging and visual helpers
	*        - Gizmo
	*        - Grid
	*        - Debug draw requests
	*        - Meter box and meter ruler
	*    - ID 100-104 (shadows disabled): "qsf::RENDER_QUEUE_OVERLAY": Used for overlays/GUI
	*      - libRocket GUI
	*    - ID 105: "qsf::RENDER_QUEUE_MAX": Unused = invisible
	*/
	enum RenderQueueGroupID
	{
		/// Use this queue for objects which must be rendered first e.g. backgrounds
		RENDER_QUEUE_BACKGROUND = 0,
		/// First queue (after backgrounds), used for skyboxes if rendered first
		RENDER_QUEUE_SKIES_EARLY = 5,
		RENDER_QUEUE_1 = 10,
		RENDER_QUEUE_2 = 20,
		RENDER_QUEUE_WORLD_GEOMETRY_1 = 25,
		RENDER_QUEUE_3 = 30,
		RENDER_QUEUE_4 = 40,
		/// The default render queue
		RENDER_QUEUE_MAIN = 50,
		RENDER_QUEUE_6 = 60,
		RENDER_QUEUE_7 = 70,
		RENDER_QUEUE_WORLD_GEOMETRY_2 = 75,
		RENDER_QUEUE_8 = 80,
		RENDER_QUEUE_9 = 90,
		/// Penultimate queue(before overlays), used for skyboxes if rendered last
		RENDER_QUEUE_SKIES_LATE = 95,
		/// Use this queue for objects which must be rendered last e.g. overlays
		RENDER_QUEUE_OVERLAY = 100,
		/// Final possible render queue, don't exceed this
		RENDER_QUEUE_MAX = 105
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
