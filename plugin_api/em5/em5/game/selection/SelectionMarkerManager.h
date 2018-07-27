// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/UniqueInstance.h>

#include <boost/signals2.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class GameDebugGroup;
}
namespace qsf
{
	class Entity;
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
	*    EMERGENCY 5 selection marker manager class that cares for activation and deactivation of markers
	*/
	class EM5_API_EXPORT SelectionMarkerManager : public qsf::UniqueInstance<SelectionMarkerManager>
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		SelectionMarkerManager();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~SelectionMarkerManager();

		void setLocalPlayerDefaultAlpha(float localPlayerDefaultAlpha);
		void showGroundMarker(qsf::Entity& entity, bool selected);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onEntitySelected(uint64 entityId);
		void onEntityDeselected(uint64 entityId);
		void updateMiniMapSquadVehiclePathVisualization(uint64 deselectedEntityId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		const GameDebugGroup&			mGameDebugGroup;
		boost::signals2::connection		mSelectSignalConnection;
		boost::signals2::connection		mDeselectSignalConnection;
		float							mLocalPlayerDefaultAlpha;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
