// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/editmode/transform/map/VertexSnapEditMode.h"


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Component;
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
		*    Vertex snap transform edit mode class
		*/
		class VertexSnapTransformEditMode : public VertexSnapEditMode
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;	///< "qsf::editor::VertexSnapTransformEditMode" unique pluginable edit mode ID


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
			explicit VertexSnapTransformEditMode(EditModeManager* editModeManager);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~VertexSnapTransformEditMode();


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::EditMode methods       ]
		//[-------------------------------------------------------]
		protected:
			virtual bool onStartup(EditMode* previousEditMode) override;
			virtual void onShutdown(EditMode* nextEditMode) override;
			virtual void mouseReleaseEvent(QMouseEvent& qMouseEvent) override;
			virtual void mouseMoveEvent(QMouseEvent& qMouseEvent) override;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void stopDrawing();
			void snapGizmo();

			/**
			*  @brief
			*    Build an operation that transform the given entity
			*
			*  @param[in] entityId
			*    ID of the entity to transform
			*  @param[in] startupTransform
			*    Transform at the point in time where the transform edit mode was started
			*
			*  @return
			*    The created operation instance, null pointer on error, destroy the operation instance if you no longer need it
			*/
			base::Operation* buildTransformOperation(uint64 entityId, const Transform& startupTransform) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			std::vector<uint64>			  mEntityIdSet;			///< IDs of the entities selected at the point in time where the transform mode startup was performed
			std::vector<Transform>		  mStartupTransform;	///< The transform of the selected entities at the point in time the transform edit mode became active
			bool						  mGizmoSnapped;
			std::vector<const Component*> mNearbyComponents;
			unsigned int				  mDebugDrawRequestId;	///< Debug draw request ID, "qsf::getUninitialized<unsigned int>()" if invalid


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::VertexSnapTransformEditMode)
