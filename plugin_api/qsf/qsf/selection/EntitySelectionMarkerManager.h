// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/selection/SelectionMarkerManager.h"
#include "qsf/platform/PlatformTypes.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class EntitySelectionManager;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    QSF entity selection marker manager class that cares for activation and deactivation of markers
	*/
	class EntitySelectionMarkerManager : public SelectionMarkerManager
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] entitySelectionManager
		*    Entity selection manager to spy on, must stay valid as long as this selection marker manager instance exists
		*/
		explicit EntitySelectionMarkerManager(EntitySelectionManager& entitySelectionManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~EntitySelectionMarkerManager();


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onSelected(uint64 id);
		void onDeselected(uint64 id);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		EntitySelectionManager* mEntitySelectionManager;	///< Entity selection manager to spy on, always valid, do not destroy the instance


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
