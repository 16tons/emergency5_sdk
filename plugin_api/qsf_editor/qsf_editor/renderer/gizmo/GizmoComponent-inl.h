// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		inline GizmoComponent::TransformationAxis GizmoComponent::axisByIndex(int index)
		{
			QSF_ASSERT(index >= 0 && index < 3, "GizmoComponent::axisByIndex(): index must be between 0 and 2", QSF_REACT_NONE);
			return (TransformationAxis)(1 << index);
		}

		inline int GizmoComponent::getNumSingleAxes(TransformationAxis axis)
		{
			return ((axis & AXIS_X) ? 1 : 0)
				 + ((axis & AXIS_Y) ? 1 : 0)
				 + ((axis & AXIS_Z) ? 1 : 0);
		}

		inline glm::vec3 GizmoComponent::getAxisVector(TransformationAxis axis)
		{
			return glm::vec3((axis & AXIS_X) ? 1.0f : 0.0f,
							 (axis & AXIS_Y) ? 1.0f : 0.0f,
							 (axis & AXIS_Z) ? 1.0f : 0.0f);
		}

		inline glm::vec3 GizmoComponent::getAxisNormalVector(TransformationAxis axis)
		{
			return glm::vec3((axis & AXIS_X) ? 0.0f : 1.0f,
							 (axis & AXIS_Y) ? 0.0f : 1.0f,
							 (axis & AXIS_Z) ? 0.0f : 1.0f);
		}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline GizmoComponent::GizmoComponent(Prototype* prototype) :
			Component(prototype),
			mShowGizmo(true),
			mGizmoSize(0.15f),
			mOgreSceneNode(nullptr),
			mOgreObject(nullptr),
			mHighlightedAxis(AXIS_NONE),
			mActiveAxis(AXIS_NONE),
			mUseLocalTransformation(false)
		{
			// Nothing to do in here
		}

		inline GizmoComponent::~GizmoComponent()
		{
			// Nothing to do in here
		}

		inline GizmoComponent::TransformationAxis GizmoComponent::getActiveAxis() const
		{
			return mActiveAxis;
		}

		inline bool GizmoComponent::getShowGizmo() const
		{
			return mShowGizmo;
		}

		inline float GizmoComponent::getGizmoSize() const
		{
			return mGizmoSize;
		}


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::GizmoComponent methods    ]
		//[-------------------------------------------------------]
		inline GizmoComponent::TransformationMode GizmoComponent::getTransformationMode() const
		{
			// Override this method in subclasses
			return TRANSFORMATION_NONE;
		}

		inline void GizmoComponent::setUseLocalTransformation(bool useLocal)
		{
			mUseLocalTransformation = useLocal;
			updateSceneNodeTransformation();
		}

		inline GizmoComponent::TransformationAxis GizmoComponent::getValidAxis(TransformationAxis axis) const
		{
			return AXIS_X;
		}

		inline void GizmoComponent::mouseMoveEvent(const Ray& ray)
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline GizmoComponent::TransformationAxis GizmoComponent::internalGetSelectedAxis(const Ray& ray, glm::vec3* hitPosition)
		{
			return AXIS_NONE;
		}

		inline void GizmoComponent::internalActivateAxis(TransformationAxis axis, const glm::vec3& initialHitPosition)
		{
			// Nothing to do in here
		}

		inline void GizmoComponent::rebuildManualObject()
		{
			// Nothing to do in here
		}


		//[-------------------------------------------------------]
		//[ GizmoComponent::AxisRating                            ]
		//[-------------------------------------------------------]
		inline GizmoComponent::AxisRating::AxisRating() :
			mValue(0.0f)
		{
			// Empty
		}

		inline void GizmoComponent::AxisRating::testAgainst(float value, const glm::vec3& hitPosition)
		{
			if (value > mValue)
			{
				mValue = value;
				mHitPosition = hitPosition;
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
