// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/Export.h"

#include <qsf/base/UniqueInstance.h>
#include <qsf/settings/SettingsGroup.h>


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
		*    QSF editor transform settings group implementation
		*/
		class QSF_EDITOR_API_EXPORT TransformSettingsGroup : public SettingsGroup, public qsf::UniqueInstance<TransformSettingsGroup>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;					///< "qsf::editor::TransformSettingsGroup" unique pluginable settings group ID
			static const uint32 SHOW_GIZMO;						///< "ShowGizmo" unique class property ID inside the class
			static const uint32 GIZMO_SIZE;						///< "GizmoSize" unique class property ID inside the class
			static const uint32 FOLLOW_GROUND;					///< "FollowGround" unique class property ID inside the class
			static const uint32 INDIVIDUAL_FOLLOW_GROUND;		///< "IndividualFollowGround" unique class property ID inside the class
			static const uint32 GRID_ENABLED;					///< "GridEnabled" unique class property ID inside the class
			static const uint32 GRID_SIZE;						///< "GridSize" unique class property ID inside the class
			static const uint32 SNAP_TO_GRID;					///< "SnapToGrid" unique class property ID inside the class
			static const uint32 SNAP_ANGLE;						///< "SnapAngle" unique class property ID inside the class
			static const uint32 LOCAL_MULTISELECTION_ROTATION;	///< "LocalMultiselectionRotation" unique class property ID inside the class
			static const uint32 INCLUDE_BONE_LINKS;				///< "IncludeBoneLinks" unique class property ID inside the class


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] settingsGroupManager
			*    Optional pointer to the settings group manager this settings group should be registered to, can be a null pointer
			*/
			inline explicit TransformSettingsGroup(SettingsGroupManager* settingsGroupManager);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~TransformSettingsGroup();

			//[-------------------------------------------------------]
			//[ Gizmo                                                 ]
			//[-------------------------------------------------------]
			inline bool getShowGizmo() const;
			inline void setShowGizmo(bool showGizmo);

			inline float getGizmoSize() const;
			inline void setGizmoSize(float gizmoSize);

			//[-------------------------------------------------------]
			//[ Movement gizmo                                        ]
			//[-------------------------------------------------------]
			inline bool getFollowGround() const;
			inline void setFollowGround(bool enabled);

			inline bool getIndividualFollowGround() const;
			inline void setIndividualFollowGround(bool enabled);

			/** Return whether grid is currently enabled */
			inline bool getGridEnabled() const;

			/** Set grid state to enabled or disabled */
			void setGridEnabled(bool enabled);

			/** Return current grid size, i.e. distance between thinnest (= primary) lines */
			inline float getGridSize() const;

			/** Set grid size, i.e. distance between primary lines */
			void setGridSize(float gridSize);

			inline bool getSnapToGrid() const;
			inline void setSnapToGrid(bool enabled);

			//[-------------------------------------------------------]
			//[ Rotate gizmo                                          ]
			//[-------------------------------------------------------]
			inline float getSnapAngle() const;
			inline void setSnapAngle(float snapAngle);
			inline bool getLocalMultiselectionRotation() const;
			inline void setLocalMultiselectionRotation(bool localMultiselectionRotation);

			//[-------------------------------------------------------]
			//[ Edit mode extensions                                  ]
			//[-------------------------------------------------------]
			inline bool getIncludeBoneLinks() const;
			inline void setIncludeBoneLinks(bool includeBoneLinks);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			bool  mShowGizmo;
			float mGizmoSize;
			bool  mFollowGround;				///< If true, follow ground mode is enabled
			bool  mIndividualFollowGround;		///< If true and follow ground is also true, follow ground mode is enabled for each individual object in a multi-selection instead of just the current selected object
			bool  mGridEnabled;					///< If true, grid rendering is enabled
			float mGridSize;					///< Size of grid in units
			bool  mSnapToGrid;					///< If true, snap to grid mode is enabled
			float mSnapAngle;					///< Snap angle for rotate gizmo, a value of 0.0f disables snapping
			bool  mLocalMultiselectionRotation;	///< If true and if multiple objects are selected, each object will rotate around its own center instead rotating around the gizmo
			bool  mIncludeBoneLinks;			///< If true and an object with a bone link is selected we will change local position and rotation too


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
#include "qsf_editor/settings/TransformSettingsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::TransformSettingsGroup)
