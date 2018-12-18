// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline TransformSettingsGroup::TransformSettingsGroup(SettingsGroupManager* settingsGroupManager) :
			SettingsGroup(settingsGroupManager),
			mShowGizmo(true),
			mGizmoSize(0.15f),
			mFollowGround(false),
			mIndividualFollowGround(true),
			mGridEnabled(true),
			mGridSize(3.0f),
			mSnapToGrid(false),
			mSnapAngle(1.0f),
			mLocalMultiselectionRotation(false),
			mIncludeBoneLinks(false)
		{
			// Nothing to do in here
		}

		inline TransformSettingsGroup::~TransformSettingsGroup()
		{
			// Nothing to do in here
		}

		inline bool TransformSettingsGroup::getShowGizmo() const
		{
			return mShowGizmo;
		}

		inline void TransformSettingsGroup::setShowGizmo(bool showGizmo)
		{
			assignAndPromoteChange(mShowGizmo, showGizmo, SHOW_GIZMO);
		}

		inline float TransformSettingsGroup::getGizmoSize() const
		{
			return mGizmoSize;
		}

		inline void TransformSettingsGroup::setGizmoSize(float gizmoSize)
		{
			assignAndPromoteChange(mGizmoSize, gizmoSize, GIZMO_SIZE);
		}

		inline bool TransformSettingsGroup::getFollowGround() const
		{
			return mFollowGround;
		}

		inline void TransformSettingsGroup::setFollowGround(bool enabled)
		{
			assignAndPromoteChange(mFollowGround, enabled, FOLLOW_GROUND);
		}

		inline bool TransformSettingsGroup::getIndividualFollowGround() const
		{
			return mIndividualFollowGround;
		}

		inline void TransformSettingsGroup::setIndividualFollowGround(bool enabled)
		{
			assignAndPromoteChange(mIndividualFollowGround, enabled, INDIVIDUAL_FOLLOW_GROUND);
		}

		inline bool TransformSettingsGroup::getGridEnabled() const
		{
			return mGridEnabled;
		}

		inline float TransformSettingsGroup::getGridSize() const
		{
			return mGridSize;
		}

		inline bool TransformSettingsGroup::getSnapToGrid() const
		{
			return mSnapToGrid;
		}

		inline void TransformSettingsGroup::setSnapToGrid(bool enabled)
		{
			assignAndPromoteChange(mSnapToGrid, enabled, SNAP_TO_GRID);
		}

		inline float TransformSettingsGroup::getSnapAngle() const
		{
			return mSnapAngle;
		}

		inline void TransformSettingsGroup::setSnapAngle(float snapSize)
		{
			assignAndPromoteChange(mSnapAngle, snapSize, SNAP_ANGLE);
		}

		inline bool TransformSettingsGroup::getLocalMultiselectionRotation() const
		{
			return mLocalMultiselectionRotation;
		}

		inline void TransformSettingsGroup::setLocalMultiselectionRotation(bool localMultiselectionRotation)
		{
			assignAndPromoteChange(mLocalMultiselectionRotation, localMultiselectionRotation, LOCAL_MULTISELECTION_ROTATION);
		}

		inline bool TransformSettingsGroup::getIncludeBoneLinks() const
		{
			return mIncludeBoneLinks;
		}

		inline void TransformSettingsGroup::setIncludeBoneLinks(bool includeBoneLinks)
		{
			assignAndPromoteChange(mIncludeBoneLinks, includeBoneLinks, INCLUDE_BONE_LINKS);
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf
