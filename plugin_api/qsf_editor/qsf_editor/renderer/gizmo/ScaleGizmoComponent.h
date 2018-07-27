// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/renderer/gizmo/GizmoComponent.h"


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
		*    Scale gizmo component class
		*/
		class ScaleGizmoComponent : public GizmoComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::editor::ScaleGizmoComponent" unique component ID


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] prototype
			*    The prototype this component is in, no null pointer allowed
			*/
			ScaleGizmoComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~ScaleGizmoComponent();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::GizmoComponent methods    ]
		//[-------------------------------------------------------]
		public:
			virtual TransformationMode getTransformationMode() const override;
			virtual TransformationAxis getValidAxis(TransformationAxis axis) const override;
			virtual void mouseMoveEvent(const Ray& ray) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::editor::GizmoComponent methods ]
		//[-------------------------------------------------------]
		protected:
			virtual TransformationAxis internalGetSelectedAxis(const Ray& ray, glm::vec3* hitPosition) override;
			virtual void internalActivateAxis(TransformationAxis axis, const glm::vec3& initialHitPosition) override;
			virtual void rebuildManualObject() override;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			glm::vec3	mOriginalScale;
			Line		mScaleAxis;


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::ScaleGizmoComponent)
