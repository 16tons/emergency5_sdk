// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/selection/entity/EntitySelectionManager.h"
#include "qsf_editor/renderer/gizmo/GizmoComponent.h"

#include <qsf/math/Transform.h>
#include <qsf/base/manager/Manager.h>
#include <qsf/math/BitFlagSet.h>

#include <QtCore/qobject.h>

#include <map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
QT_BEGIN_NAMESPACE
	class QMouseEvent;
QT_END_NAMESPACE
namespace qsf
{
	class RenderWindow;
	class ParameterGroup;
	class TransformComponent;
	namespace editor
	{
		class GridComponent;
		class TransformEditMode;
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
		*    Gizmo manager class
		*
		*  @remarks
		*    The gizmo manager manages the helper entity containing the gizmo components. All other editor parts
		*    only access the gizmo manager and never the helper entity nor the gizmo components directly.
		*/
		class QSF_EDITOR_API_EXPORT GizmoManager : public QObject, public Manager
		{


		//[-------------------------------------------------------]
		//[ Qt definitions (MOC)                                  ]
		//[-------------------------------------------------------]
		Q_OBJECT	// All files using the Q_OBJECT macro need to be compiled using the Meta-Object Compiler (MOC) of Qt, else slots won't work!
					// (VisualStudio: Header file -> Right click -> Properties -> "Custom Build Tool")


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Feature set flags
			*/
			enum Feature
			{
				MOVEMENT_FEATURE = 1<<0,	///< Gizmo movement possible
				ROTATE_FEATURE	 = 1<<1,	///< Gizmo rotation possible
				SCALE_FEATURE	 = 1<<2,	///< Gizmo scale possible
				LOCAL_FEATURE	 = 1<<3		///< Local transform possible
			};
			typedef BitFlagSet<uint8, Feature> FeatureBitmask;


		//[-------------------------------------------------------]
		//[ Public Qt signals (MOC)                               ]
		//[-------------------------------------------------------]
		Q_SIGNALS:
			/**
			*  @brief
			*    Qt signal emitted after a gizmo changed occurred
			*
			*  @todo
			*    - TODO(co) Review this signal and remove it if possible
			*/
			void change();

			/**
			*  @brief
			*    Qt signal emitted after a gizmo transform changed occurred
			*
			*  @param[in] transform
			*    The new transform of the gizmo
			*/
			void transformChange(const Transform& transform);


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			GizmoManager();

			/**
			*  @brief
			*    Destructor
			*/
			virtual ~GizmoManager();

			// TODO(co) Comment, see "qsf::editor::GizmoManager::Feature"
			const FeatureBitmask& getFeatureSet() const;
			void setFeatureSet(const FeatureBitmask& flags);

			// TODO(co) Comment
			void resetDefaultTransformationAxes();

			// TODO(co) Comment
			bool isGizmoEnabled() const;
			void setGizmoEnabled(bool gizmoEnabled);

			// TODO(co) Comment
			const Transform& getGizmoTransform();
			void setGizmoTransform(const Transform& transform);

			// TODO(co) Comment
			GizmoComponent::TransformationMode getActiveMode() const;
			void setActiveMode(GizmoComponent::TransformationMode mode);

			// TODO(co) Comment
			GizmoComponent::TransformationAxis getActiveAxis() const;
			/*
			*  @param[in] initialHitPosition
			*    The position in world space where the transformation change should be started; this may be an intersection position
			*    with the gizmo (from getSelectedAxis()) or any other position, also outside of the gizmo itself
			*/
			void setActiveAxis(GizmoComponent::TransformationAxis axis, const glm::vec3& initialHitPosition);
			void toggleActiveAxis(GizmoComponent::TransformationAxis axis, const glm::vec3& initialHitPosition);

			// TODO(co) Comment
			bool getUseLocalTransformation() const;
			void setUseLocalTransformation(bool useLocal);

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
			GizmoComponent::TransformationAxis getValidAxis(GizmoComponent::TransformationAxis axis);

			/**
			*  @brief
			*    Return the entities to be ignored when performing follow ground
			*
			*  @param[out] idSet
			*    Receives the entities to be ignored when performing follow ground, set is not cleared before adding new IDs
			*/
			void getFollowGroundIgnoreEntityIds(EntitySelectionManager::IdSet& idSet);

			/**
			*  @brief
			*    Set the entities to be ignored when performing follow ground
			*
			*  @param[in] ignoreEntityIds
			*    The given entities will be ignored for the height calculation
			*/
			void setFollowGroundIgnoreEntityIds(const EntitySelectionManager::IdSet& ignoreEntityIds);

			/**
			*  @brief
			*    Update the gizmo axis highlighting
			*
			*  @param[in] qMouseEvent
			*    Qt mouse event to process
			*  @param[in] renderWindow
			*    Render window to use
			*/
			void updateAxisHighlighting(QMouseEvent& qMouseEvent, RenderWindow& renderWindow);

			// TODO(co) Comment
			bool switchToTransformEditModeIfPossible(QMouseEvent& qMouseEvent, RenderWindow& renderWindow, TransformEditMode& transformEditMode);
			void switchToTransformEditMode(TransformEditMode& transformEditMode, GizmoComponent::TransformationAxis selectedAxis, const glm::vec3& hitPosition, Qt::KeyboardModifiers qtKeyboardModifiers);

			//[-------------------------------------------------------]
			//[ QWidget methods to tell the gizmo what's going on     ]
			//[-------------------------------------------------------]
			void mouseReleaseEvent(QMouseEvent& qMouseEvent, RenderWindow& renderWindow);
			void mouseMoveEvent(QMouseEvent& qMouseEvent, RenderWindow& renderWindow);

			//[-------------------------------------------------------]
			//[ Grid - Used to support precise gizmo transform        ]
			//[-------------------------------------------------------]
			/** Return whether grid is currently enabled */
			bool getGridEnabled();

			/** Set grid state to enabled or disabled */
			void setGridEnabled(bool enabled);

			/** Return current grid size, i.e. distance between thinnest (= primary) lines */
			float getGridSize();

			/** Set grid size, i.e. distance between primary lines */
			void setGridSize(float gridSize);

			/** In case the grid is currently enabled, snap the given world space position to the grid */
			glm::vec3 snapPositionToGrid(const glm::vec3& position);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Return or create the gizmo entity
			*
			*  @return
			*    The gizmo entity instance, null pointer on error, do not destroy the instance
			*/
			Entity* getOrCreateGizmoEntity();

			/**
			*  @brief
			*    Helper function to destroy an entity's gizmo component
			*
			*  @param[in] entity
			*    Pointer to the entity. All gizmo components found in there will be destroyed. Can be a null pointer.
			*/
			void destroyGizmoComponents(Entity* entity);

			// TODO(co) Comment
			void removeGizmoComponentById(Entity* entity, uint32 componentId);
			GizmoComponent* getOrCreateGizmoComponent();
			TransformComponent* getOrCreateTransformComponent();
			void updateGizmoVisibility();

			// TODO(co) Comment
			GridComponent* getOrCreateGridComponent();


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Called as soon as there's a property change inside the settings group manager
			*
			*  @param[in] parameterGroup
			*    Parameter group instance the changed property is in
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*/
			void onSettingsGroupPropertyChanged(const ParameterGroup& parameterGroup, uint32 propertyId);

			/**
			*  @brief
			*    Called in case a component property value was changed
			*
			*  @param[in] component
			*    Component instance which had a property value change
			*  @param[in] propertyId
			*    Unique class property ID the component property which values was changed
			*
			*  @note
			*    - The unique class property identifier is generated by "qsf::StringHash" by using the class property name
			*      (e.g. qsf::StringHash("Position") generating the hash value 3091144222)
			*/
			void onComponentPropertyChange(const Component& component, uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Private definitions                                   ]
		//[-------------------------------------------------------]
		private:
			// Mode specific settings
			struct ModeSpecificSettings
			{
				GizmoComponent::TransformationAxis mDefaultAxis;
				GizmoComponent::TransformationAxis mActiveAxis;
				bool mLocalTransformation;
			};
			typedef std::map<GizmoComponent::TransformationMode, ModeSpecificSettings> ModeSpecificSettingsMap;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			FeatureBitmask					   mFeatureSet;				///< Active gizmo feature set
			WeakPtr<Entity>					   mGizmoEntity;			///< Gizmo entity weak pointer
			bool							   mGizmoEnabled;			///< Is the gizmo currently enabled?
			Transform						   mFallbackTransform;		///< Fallback transform used in case there's no gizmo entity
			GizmoComponent::TransformationMode mActiveMode;				///< Currently active gizmo mode
			ModeSpecificSettingsMap			   mModeSpecificSettings;	///< Mode specific settings map


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
