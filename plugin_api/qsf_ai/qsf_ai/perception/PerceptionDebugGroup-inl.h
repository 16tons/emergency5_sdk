// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline PerceptionDebugGroup::PerceptionDebugGroup(DebugGroupManager* dgm) :
			DebugGroup(dgm),
			mShowLinesOfSight(DEFAULT_SHOW_LINES_OF_SIGHT),
			mShowObstructedLinesOfSight(DEFAULT_SHOW_OBSTRUCTED_LINES_OF_SIGHT),
			mMarkPerceivedEntities(DEFAULT_MARK_PERCEIVED_ENTITIES),
			mMarkPerceivers(DEFAULT_MARK_PERCEIVERS),
			mShowSensorArea(DEFAULT_SHOW_SENSOR_AREA),
			mShowForAllEntities(DEFAULT_SHOW_FOR_ALL_ENTITIES)
		{
		}

		inline bool PerceptionDebugGroup::getShowLinesOfSight() const
		{
			return mShowLinesOfSight;
		}

		inline void PerceptionDebugGroup::setShowLinesOfSight(bool showLinesOfSight)
		{
			assignAndPromoteChange(mShowLinesOfSight,showLinesOfSight,SHOW_LINES_OF_SIGHT_PROPERTY_ID);
		}

		inline bool PerceptionDebugGroup::getShowObstructedLinesOfSight() const
		{
			return mShowObstructedLinesOfSight;
		}

		inline void PerceptionDebugGroup::setShowObstructedLinesOfSight(bool showObstructedLinesOfSight)
		{
			assignAndPromoteChange(mShowObstructedLinesOfSight,showObstructedLinesOfSight,SHOW_OBSTRUCTED_LINES_OF_SIGHT_PROPERTY_ID);
		}

		inline bool PerceptionDebugGroup::getMarkPerceivedEntities() const
		{
			return mMarkPerceivedEntities;
		}

		inline void PerceptionDebugGroup::setMarkPerceivedEntities(bool markPerceivedEntities)
		{
			assignAndPromoteChange(mMarkPerceivedEntities,markPerceivedEntities,MARK_PERCEIVED_ENTITIES_PROPERTY_ID);
		}

		inline bool PerceptionDebugGroup::getMarkPerceivers() const
		{
			return mMarkPerceivers;
		}

		inline void PerceptionDebugGroup::setMarkPerceivers(bool markPerceivers)
		{
			assignAndPromoteChange(mMarkPerceivers,markPerceivers,MARK_PERCEIVERS_PROPERTY_ID);
		}

		inline bool PerceptionDebugGroup::getShowSensorArea() const
		{
			return mShowSensorArea;
		}

		inline void PerceptionDebugGroup::setShowSensorArea(bool showSensorArea)
		{
			assignAndPromoteChange(mShowSensorArea,showSensorArea,SHOW_SENSOR_AREA_PROPERTY_ID);
		}

		inline bool PerceptionDebugGroup::getShowForAllEntities() const
		{
			return mShowForAllEntities;
		}

		inline void PerceptionDebugGroup::setShowForAllEntities(bool showForAllEntities)
		{
			assignAndPromoteChange(mShowForAllEntities,showForAllEntities,SHOW_FOR_ALL_ENTITIES_PROPERTY_ID);
		}

//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
