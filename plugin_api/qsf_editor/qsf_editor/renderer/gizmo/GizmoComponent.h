// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/math/Ray.h>
#include <qsf/math/Plane.h>
#include <qsf/math/Color4.h>
#include <qsf/component/Component.h>

#include <glm/fwd.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Ogre
{
	class Matrix3;
	class SceneNode;
	class ManualObject;
}
namespace qsf
{
	class Transform;
	class CameraComponent;
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
		*    Gizmo component base class
		*/
		class GizmoComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::editor::GizmoComponent" unique component ID

			enum TransformationMode
			{
				TRANSFORMATION_NONE = 0,
				TRANSFORMATION_MOVEMENT,
				TRANSFORMATION_ROTATE,
				TRANSFORMATION_SCALE
			};

			enum TransformationAxis
			{
				AXIS_NONE = 0,
				AXIS_X = 1,
				AXIS_Y = 2,
				AXIS_Z = 4,
				AXIS_X_Y = AXIS_X | AXIS_Y,
				AXIS_X_Z = AXIS_X | AXIS_Z,
				AXIS_Y_Z = AXIS_Y | AXIS_Z,
				AXIS_UNIFORM = AXIS_X | AXIS_Y | AXIS_Z
			};


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Helper function to enumerate axes
			*
			*  @param[in] index
			*    The axis' index from 0 (for AXIS_X) to 2 (for AXIS_Z); other values are not allowed
			*
			*  @return
			*    The axis for the given index; may only be AXIS_X, AXIS_Y or AXIS_Z
			*/
			inline static TransformationAxis axisByIndex(int index);

			/**
			*  @brief
			*    Helper function to count number of axes
			*
			*  @param[in] axis
			*    An axis enumerator of type TransformationAxis
			*
			*  @return
			*    The number of single axes (i.e. one for every axis of AXIS_X, AXIS_Y, AXIS_Z - summed up). Between 0 and 3.
			*/
			inline static int getNumSingleAxes(TransformationAxis axis);

			/**
			*  @brief
			*    Helper function to get a vector in an axis's direction.
			*
			*  @param[in] axis
			*    The axis for which a vector representation is needed. All valid values in enum TransformationAxis are accepted.
			*
			*  @return
			*    A unit vector for AXIS_X, AXIS_Y, AXIS_Z, a sum of two or all three unit vectors for more "higher" axis values, or zero vector for AXIS_NONE.
			*/
			inline static glm::vec3 getAxisVector(TransformationAxis axis);

			/**
			*  @brief
			*    Helper function to get a vector orthogonal to an axis's direction (at least this is the result for AXIS_X_Y, AXIS_X_Z, AXIS_Y_Z).
			*
			*  @param[in] axis
			*    The axis for which a normal vector representation is needed. All valid values in enum TransformationAxis are accepted.
			*
			*  @return
			*    The resulting vector. This is equal to glm::vec3(1.0f, 1.0f, 1.0f) - getAxisVector(axis).
			*/
			inline static glm::vec3 getAxisNormalVector(TransformationAxis axis);


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
			inline GizmoComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~GizmoComponent();

			/**
			*  @brief
			*    Determine the axis hit by the given ray
			*
			*  @param[in] ray
			*    The ray in world space (e.g. ray under the mouse cursor)
			*  @param[out] hitPosition
			*    Pointer to a position to return the axis or axis plane of the gizmo that was hit, or nullptr
			*
			*  @return
			*    The axis that was hit by the ray, or AXIS_NONE if none was hit
			*/
			TransformationAxis getSelectedAxis(const Ray& ray, glm::vec3* hitPosition = nullptr);

			/**
			*  @brief
			*    Highlight an axis; this only affects the gizmo's graphic representation
			*
			*  @param[in] axis
			*    The axis to be highlighted, AXIS_NONE is a valid value
			*/
			void highlightAxis(TransformationAxis axis);

			/**
			*  @brief
			*    Activate an axis, so that further calls to mouseMoveEvent() will lead to a change in entity's transformation
			*
			*  @param[in] axis
			*    The axis to be activated, or AXIS_NONE
			*  @param[in] initialHitPosition
			*    The position in world space where the transformation change should be started; this may be an intersection position
			*    with the gizmo (from getSelectedAxis()) or any other position, also outside of the gizmo itself
			*
			*  @note
			*    - You might want to use "qsf::editor::GizmoComponent::getValidAxis()" before activating an axis
			*/
			void activateAxis(TransformationAxis axis, const glm::vec3& initialHitPosition);

			/**
			*  @brief
			*    Deactivate gizmo, so there is no axis active any more and mouseMoveEvent() won't change entity's transformation.
			*/
			void deactivate();

			/**
			*  @brief
			*    Return active axis
			*
			*  @return
			*    Active axis set with activateAxis(), or AXIS_NONE if no axis currently activated
			*/
			inline TransformationAxis getActiveAxis() const;

			inline bool getShowGizmo() const;
			void setShowGizmo(bool showGizmo);

			inline float getGizmoSize() const;
			void setGizmoSize(float gizmoSize);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::GizmoComponent methods    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Get the gizmo's transformation mode
			*
			*  @return
			*    The transformation mode, which is unique for every GizmoComponent subclass
			*/
			inline virtual TransformationMode getTransformationMode() const;

			inline virtual void setUseLocalTransformation(bool useLocal);

			/**
			*  @brief
			*    Check if given axis is valid for this gizmo and return default axis if it isn't
			*
			*  @param[in] axis
			*    Transformation axis to be checked
			*
			*  @return
			*    The input parameter if it was a valid axis, otherwise the default axis (which is always the case for parameter AXIS_NONE)
			*/
			inline virtual TransformationAxis getValidAxis(TransformationAxis axis) const;

			/**
			*  @brief
			*    Call when mouse has moved in viewport and gizmo is active, i.e. user is currently changing the transformation via the gizmo
			*
			*  @param[in] ray
			*    The ray under the mouse cursor in world space
			*/
			inline virtual void mouseMoveEvent(const Ray& ray);


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			inline virtual bool implementsOnComponentPropertyChange() const override  { return true; }
			virtual void onComponentTransformChange(const Transform& transform, TransformChangeFlags flags) override;

			//[-------------------------------------------------------]
			//[ Lifecycle                                             ]
			//[-------------------------------------------------------]
			virtual bool onStartup() override;
			virtual void onShutdown() override;

			virtual void onSetHidden(bool hidden) override;


		//[-------------------------------------------------------]
		//[ Protected static methods                              ]
		//[-------------------------------------------------------]
		protected:
			static Color4 getDefaultAxisColorByIndex(int index, bool transparent = false);
			static Color4 getDisabledAxisColor(int index);
			static Color4 getHighlightedAxisColor();
			static void buildAxisTransformationMatrices(Ogre::Matrix3* axisTransformations);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			inline virtual TransformationAxis internalGetSelectedAxis(const Ray& ray, glm::vec3* hitPosition = nullptr);

			// Throws exceptions
			inline virtual void internalActivateAxis(TransformationAxis axis, const glm::vec3& initialHitPosition);

			inline virtual void rebuildManualObject();

			void updateSceneNodeTransformation(const Transform* transform = nullptr, bool updatePosition = true, bool updateRotation = true);


		//[-------------------------------------------------------]
		//[ Protected definitions                                 ]
		//[-------------------------------------------------------]
		protected:
			static const std::string MATERIAL_NAME;	///< Name of Ogre material used for our manual object

			// Internal helper structure for determination of highlighted/selected axis
			struct AxisRating
			{
				float mValue;
				glm::vec3 mHitPosition;

				inline AxisRating();
				inline void testAgainst(float value, const glm::vec3& hitPosition);
			};


		//[-------------------------------------------------------]
		//[ Protected data                                        ]
		//[-------------------------------------------------------]
		protected:
			bool					mShowGizmo;			///< If false, OGRE scene node will be hidden, and gizmo axes won't be selectable any more
			float					mGizmoSize;			///< Gizmo size
			Ogre::SceneNode*		mOgreSceneNode;		///< OGRE scene node instance containing mOgreObject
			Ogre::ManualObject*		mOgreObject;		///< OGRE object instance holding gizmo mesh
			TransformationAxis		mHighlightedAxis;	///< Currently highlighted gizmo axis (usually per mouse over)
			TransformationAxis		mActiveAxis;		///< Active gizmo axis, while transformation with gizmo is done, else AXIS_NONE
			bool					mUseLocalTransformation;

			boost::signals2::connection mPreCompositorWorkspaceUpdateConnection;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    This method gets called before a compositor workspace gets rendered
			*
			*  @param[in] cameraComponent
			*    Used camera component
			*/
			void onPreCompositorWorkspaceUpdate(const CameraComponent& cameraComponent);


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/renderer/gizmo/GizmoComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::GizmoComponent)
