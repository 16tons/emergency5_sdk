// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/selection/SelectionSystem.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		class GizmoManager;
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
		*    Editor selection system class
		*/
		class QSF_EDITOR_API_EXPORT SelectionSystem : public qsf::SelectionSystem
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			SelectionSystem();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~SelectionSystem();

			/**
			*  @brief
			*    Return the gizmo manager instance
			*
			*  @return
			*    The gizmo manager instance, do not destroy the instance
			*/
			inline GizmoManager& getGizmoManager() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			GizmoManager* mGizmoManager;	///< Gizmo manager instance, always valid, destroy the instance if you no longer need it


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/SelectionSystem-inl.h"
