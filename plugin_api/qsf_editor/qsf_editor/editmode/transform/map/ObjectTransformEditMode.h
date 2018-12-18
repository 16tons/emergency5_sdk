// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/transform/TransformEditMode.h"
#include "qsf_editor/renderer/gizmo/GizmoComponent.h"

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{
		namespace base
		{
			class Operation;
		}
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
		*    Object transform edit mode class
		*/
		class QSF_EDITOR_API_EXPORT ObjectTransformEditMode : public TransformEditMode
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::ObjectTransformEditMode" unique pluginable edit mode ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] editModeManager
			*    The edit mode manager this edit mode instance is in, no null pointer allowed (the CAMP reflection system does not like references in constructors, so we have to use a pointer instead)
			*/
			explicit ObjectTransformEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ObjectTransformEditMode();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::TransformEditMode methods ]
		//[-------------------------------------------------------]
		protected:
			virtual void onTransformChange(const Transform& transform) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void createEntityClones();

			/**
			*  @brief
			*    Build an operation that transform the given entity
			*
			*  @param[in] entityId
			*    ID of the entity to transform
			*  @param[in] startupTransform
			*    Transform at the point in time where the transform edit mode was started
			*  @param[in] transformMode
			*    Transform mode to build an operation for
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			base::Operation* buildTransformOperation(uint64 entityId, const Transform& startupTransform, GizmoComponent::TransformationMode transformMode) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<uint64>	   mEntityIdSet;		///< IDs of the entities selected at the point in time where the transform mode startup was performed
			std::vector<Transform> mStartupTransform;	///< The transform of the selected entities at the point in time the transform edit mode became active
			std::vector<uint64>	   mPreviewEntityIds;	///< IDs of the preview entities


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ObjectTransformEditMode)
