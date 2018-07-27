// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/renderer/gizmo/GizmoComponent.h"

#include <glm/gtc/quaternion.hpp>


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
		*    Rotate gizmo component class
		*/
		class RotateGizmoComponent : public GizmoComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::editor::RotateGizmoComponent" unique component ID


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
			RotateGizmoComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~RotateGizmoComponent();

			/**
			*  @brief
			*    Set snap angle, or 0.0f to deactivate snapping.
			*
			*  @param[in] snapAngle
			*    The snap angle in degrees. Values above 0.0f will activate snapping, otherwise snapping is disabled.
			*/
			void setSnapAngle(float snapAngle);


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
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/** Return angle corresponding to given world position (e.g. mouse ray hit position) */
			float getAngleAtWorldPosition(const glm::vec3& position) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			glm::quat	mOriginalRotation;

			Line		mRotationAxis;
			Plane		mRotationPlane;
			glm::vec3	mRotationAxisLocalUp;		///< Local rotation axes, they build the "rotation frame", e.g. a local coordinate system in which angles for the current rotation are expressed
			glm::vec3	mRotationAxisLocalRight;

			float		mStartMouseAngle;			///< Rotation angle (radians) at mouse cursor when axis was activated, inside rotation frame
			float		mStartObjectAngle;			///< Rotation angle (radians) of object when axis was activated, inside rotation frame
			float		mSnapAngle;					///< Snap angle in degrees, can be 0.0f to disable snapping


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::RotateGizmoComponent)
