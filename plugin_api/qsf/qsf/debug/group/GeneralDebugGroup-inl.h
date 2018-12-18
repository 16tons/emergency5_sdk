// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline GeneralDebugGroup::GeneralDebugGroup(DebugGroupManager* debugGroupManager) :
		DebugGroup(debugGroupManager),
		#ifdef ENDUSER
			mShowFramesPerSecond(false),
		#else
			mShowFramesPerSecond(true),
		#endif
		mShowFramerateStability(true),
		mShowCollision(false),
		mShowLinkedChildren(true),
		mShowPolygon(SHOW_DEBUG),
		mShowPath(SHOW_DEBUG),
		mIgnoreEnforcedApplicationCoreProjectLock(false)
	{
		// Nothing to do in here
	}

	inline GeneralDebugGroup::~GeneralDebugGroup()
	{
		// Nothing to do in here
	}

	inline bool GeneralDebugGroup::getShowFramesPerSecond() const
	{
		return mShowFramesPerSecond;
	}

	inline void GeneralDebugGroup::setShowFramesPerSecond(bool showFramesPerSecond)
	{
		assignAndPromoteChange(mShowFramesPerSecond, showFramesPerSecond, SHOW_FRAMES_PER_SECOND);
	}

	inline bool GeneralDebugGroup::getShowFramerateStability() const
	{
		return mShowFramerateStability;
	}

	inline void GeneralDebugGroup::setShowFramerateStability(bool showFramerateStability)
	{
		assignAndPromoteChange(mShowFramerateStability, showFramerateStability, SHOW_FRAMERATE_STABILITY);
	}

	inline bool GeneralDebugGroup::getShowCollision() const
	{
		return mShowCollision;
	}

	inline bool GeneralDebugGroup::getShowLinkedChildren() const
	{
		return mShowLinkedChildren;
	}

	inline void GeneralDebugGroup::setShowLinkedChildren(bool showLinkedChildren)
	{
		assignAndPromoteChange(mShowLinkedChildren, showLinkedChildren, SHOW_LINKED_CHILDREN);
	}

	inline GeneralDebugGroup::DrawMode GeneralDebugGroup::getShowPolygon() const
	{
		return mShowPolygon;
	}

	inline void GeneralDebugGroup::setShowPolygon(DrawMode showPolygon)
	{
		assignAndPromoteChange(mShowPolygon, showPolygon, SHOW_POLYGON);
	}

	inline GeneralDebugGroup::DrawMode GeneralDebugGroup::getShowPath() const
	{
		return mShowPath;
	}

	inline void GeneralDebugGroup::setShowPath(DrawMode showPath)
	{
		assignAndPromoteChange(mShowPath, showPath, SHOW_PATH);
	}

	inline bool GeneralDebugGroup::getIgnoreEnforcedApplicationCoreProjectLock() const
	{
		return mIgnoreEnforcedApplicationCoreProjectLock;
	}

	inline bool GeneralDebugGroup::getIgnoreEnforcedApplicationCoreProjectLockInternal() const
	{
		// In end-user builds, application core projects are always locked
		// -> Good - but at the same time bad. In case we publish a broken asset package one can't fix it locally in order to be able to create an online editor server.
		// -> Added a secret settings option to be able to disable this security feature for such nasty situations.
		#ifdef ENDUSER
			return mIgnoreEnforcedApplicationCoreProjectLock;
		#else
			return true;
		#endif
	}

	inline void GeneralDebugGroup::setIgnoreEnforcedApplicationCoreProjectLock(bool ignoreEnforcedApplicationCoreProjectLock)
	{
		assignAndPromoteChange(mIgnoreEnforcedApplicationCoreProjectLock, ignoreEnforcedApplicationCoreProjectLock, IGNORE_ENFORCED_APPLICATION_CORE_PROJECT_LOCK);
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
