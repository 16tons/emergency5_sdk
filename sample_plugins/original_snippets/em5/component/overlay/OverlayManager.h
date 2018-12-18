// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/base/manager/Manager.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class OverlayComponent;
}
namespace qsf
{
	class CameraComponent;
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
	*    EMERGENCY 5 overlay manager class
	*/
	class EM5_API_EXPORT OverlayManager : public qsf::Manager
	{


	//[-------------------------------------------------------]
	//[ Friends                                               ]
	//[-------------------------------------------------------]
		friend class OverlayComponent;


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		OverlayManager();

		/**
		*  @brief
		*    Destructor
		*/
		~OverlayManager();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onPreCompositorWorkspaceUpdate(const qsf::CameraComponent& cameraComponent);

		// Only for friends
		void addVisibleOverlayComponent(OverlayComponent& overlayComponent);
		void removeVisibleOverlayComponent(OverlayComponent& overlayComponent);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Linked list of visible overlay components
		struct DoubleLinkedList
		{
			OverlayComponent* mFirst;
			DoubleLinkedList() : mFirst(nullptr) {}
		};
		DoubleLinkedList mVisibleOverlayComponents;

		boost::signals2::connection mPreCompositorWorkspaceUpdateConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
