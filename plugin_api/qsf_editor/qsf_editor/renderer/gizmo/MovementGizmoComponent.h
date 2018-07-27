// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/renderer/gizmo/GizmoComponent.h"

#include <qsf/selection/EntitySelectionManager.h>


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
		*    Movement gizmo component class
		*/
		class MovementGizmoComponent : public GizmoComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 COMPONENT_ID;	///< "qsf::editor::MovementGizmoComponent" unique component ID


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
			MovementGizmoComponent(Prototype* prototype);

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~MovementGizmoComponent();

			/**
			*  @brief
			*    Set grid snap size, or 0.0f to deactivate snapping.
			*
			*  @param[in] snapSize
			*    The grid snap size in units (meters). Values above 0.0f will activate snapping for global transformation, otherwise snapping is disabled.
			*/
			void setGridSnapSize(float snapSize);

			/**
			*  @brief
			*    Enable or disable follow ground, i.e. snapping to the terrain surface in y-direction
			*
			*  @param[in] enable
			*    If true, snapping will be enabled, or disabled otherwise
			*
			*  @note
			*    - Clears the list of ignored entities, you might want to call "qsf::editor::MovementGizmoComponent::setFollowGroundIgnoreEntityIds()" as soon as you have the required information
			*/
			void setFollowGround(bool enable);

			/**
			*  @brief
			*    Return the entities to be ignored when performing follow ground
			*
			*  @return
			*    The entities to be ignored when performing follow ground
			*/
			const EntitySelectionManager::IdSet& getFollowGroundIgnoreEntityIds() const;

			/**
			*  @brief
			*    Set the entities to be ignored when performing follow ground
			*
			*  @param[in] ignoreEntityIds
			*    The given entities will be ignored for the height calculation
			*/
			void setFollowGroundIgnoreEntityIds(const EntitySelectionManager::IdSet& ignoreEntityIds);


		//[-------------------------------------------------------]
		//[ Public virtual qsf::editor::GizmoComponent methods    ]
		//[-------------------------------------------------------]
		public:
			virtual TransformationMode getTransformationMode() const override;
			virtual void setUseLocalTransformation(bool useLocal) override;
			virtual TransformationAxis getValidAxis(TransformationAxis axis) const override;
			virtual void mouseMoveEvent(const Ray& ray) override;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::Component methods              ]
		//[-------------------------------------------------------]
		protected:
			virtual void onComponentPropertyChange(const Component& component, uint32 propertyId) override;


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
			glm::vec3	mOriginalPosition;
			glm::vec3	mGrabOffset;

			bool		mMovementAlongAxis;
			Line		mMovementAxis;

			bool		mMovementOnPlane;
			Plane		mMovementPlane;

			float						  mSnapGridSize;	///< Snap grid size in length units (meters), can be 0.0f to disable snapping
			bool						  mFollowGround;	///< If true, follow ground is active
			EntitySelectionManager::IdSet mIgnoreEntityIds;	///< The given entities will be ignored for the follow ground height calculation

			bool		mTransformationChangeByMe;	///< Set to true around call to TransformComponent when changing transformation


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
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::MovementGizmoComponent)
