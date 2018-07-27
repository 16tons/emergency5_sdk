// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline bool GameDebugGroup::getShowEventDebug() const
	{
		return mShowEventDebug;
	}

	inline bool GameDebugGroup::getShowHealthState() const
	{
		return mShowHealthState;
	}

	inline bool GameDebugGroup::getShowContaminationRadius() const
	{
		return mShowContaminationRadius;
	}

	inline bool GameDebugGroup::getShowFireDebug() const
	{
		return mShowFireDebug;
	}

	inline bool GameDebugGroup::getShowFireRadius() const
	{
		return mShowFireRadius;
	}

	inline bool GameDebugGroup::getShowFireReceiver() const
	{
		return mShowFireReceiver;
	}

	inline bool GameDebugGroup::getHideUserInterface() const
	{
		return mHideUserInterface;
	}

	inline const qsf::AssetProxy& GameDebugGroup::getStartupMap() const
	{
		return mStartupMap;
	}

	inline bool GameDebugGroup::getActiveFreeplay() const
	{
		return mActiveFreeplay;
	}

	inline bool GameDebugGroup::getShowEventDensity() const
	{
		return mShowEventDensity;
	}

	inline bool GameDebugGroup::getShowEventFailConditions() const
	{
		return mShowEventFailConditions;
	}

	inline bool GameDebugGroup::getShowAutomatismDebug() const
	{
		return mShowAutomatismDebug;
	}

	inline bool GameDebugGroup::getShowActivitiesDebug() const
	{
		return mShowActivitiesDebug;
	}

	inline bool GameDebugGroup::getShowDebugSelectionMarker() const
	{
		return mShowDebugSelectionMarker;
	}

	inline bool GameDebugGroup::getShowDebugTextComponent() const
	{
		return mShowDebugTextComponent;
	}

	inline qsf::LogMessage::SeverityLevel GameDebugGroup::getShowLogSeverityLevel() const
	{
		return mShowLogSeverityLevel;
	}

	inline void GameDebugGroup::setShowLogSeverityLevel(qsf::LogMessage::SeverityLevel showLogSeverityLevel)
	{
		mShowLogSeverityLevel = showLogSeverityLevel;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
