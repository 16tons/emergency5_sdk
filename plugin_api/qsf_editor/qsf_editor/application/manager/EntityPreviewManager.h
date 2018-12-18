// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/base/manager/Manager.h>

#include <glm/fwd.hpp>

#include <boost/container/flat_set.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
	class Prototype;
	namespace editor
	{
		class OperationManager;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Entity preview manager class
		*/
		class QSF_EDITOR_API_EXPORT EntityPreviewManager : public QObject, public Manager
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef boost::container::flat_set<uint64> EntityIdSet;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			EntityPreviewManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~EntityPreviewManager();

			/**
			*  @brief
			*    Check whether the given entity ID belongs to an entity preview
			*
			*  @param[in] entityId
			*    Entity ID to check
			*
			*  @return
			*    "true" if the provided entity ID belongs to an entity preview
			*/
			bool isEntityPreview(uint64 entityId) const;

			/**
			*  @brief
			*    Create an entity preview instance
			*
			*  @param[in] prototype
			*    Prototype instance to create a entity preview instance from
			*  @param[in] position
			*    World space position to create the entity preview instance at
			*
			*  @return
			*    The entity ID of the created entity preview instance
			*
			*  @note
			*    - Do always destroy the created entity preview instance by using "qsf::editor::EntityPreviewManager::destroyEntityPreview()"
			*/
			uint64 createEntityPreview(Prototype& prototype, const glm::vec3& position);

			/**
			*  @brief
			*    Destroy an entity preview instance
			*
			*  @param[in] entityId
			*    ID of the entity preview to destroy
			*
			*  @return
			*    "true" if all went fine, else "false"
			*/
			bool destroyEntityPreview(uint64 entityId);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			Map&			  mMap;					///< Map instance to work with, do not destroy the instance
			OperationManager& mOperationManager;	///< Operation manager instance to work with, do not destroy the instance
			EntityIdSet		  mEntityPreviewIds;	///< Entity previews IDs


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
