// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/map/Map.h>
#include <qsf/physics/PhysicsWorldComponent.h>
#include <qsf/component/base/TransformComponent.h>


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
		inline ProcessingParameters::ProcessingParameters(const Entity& perceiver,const JobArguments* jobArguments,uint32 eventId) :
			mPerceiver(perceiver),
			mPerceiverTransform(perceiver.getComponent<TransformComponent>()),
			mPhysicsWorldComponent(nullptr),
			mJobArguments(jobArguments),
			mEventId(eventId)
		{
			QSF_CHECK(mPerceiverTransform, "entity " << perceiver.getId() << " has no transform, it can't be used as a perceiver with a sensor", return);

			// TODO(co) Please, no such complex inline implementations which require to include complex headers (header complexity explosion = horrible compile time)
			mPhysicsWorldComponent = perceiver.getMap().getCoreEntity().getComponent<PhysicsWorldComponent>();
			QSF_CHECK(mPhysicsWorldComponent, "SensorComponent requires PhysicsWorldComponent at the core entity", return);
		}

		inline SensorConfiguration::SensorConfiguration() :
			mFlags(
				(DEFAULT_AUTOMATIC_UPDATE ? (1 << AUTOMATIC_UPDATE) : 0) |
				(DEFAULT_INTERPRETE_ASPECTS_AS_CLASS_NAMES ? (1 << INTERPRETE_ASPECTS_AS_CLASS_NAMES) : 0) |
				(DEFAULT_LINE_OF_SIGHT_REQUIRED ? (1 << LINE_OF_SIGHT_REQUIRED) : 0) |
				(DEFAULT_RECORD_ENDING_PERCEPTIONS ? (1 << RECORD_ENDING_PERCEPTIONS) : 0) |
				(DEFAULT_EVENT_ON_PERCEPTION_BEGIN ? (1 << EVENT_ON_PERCEPTION_BEGIN) : 0) |
				(DEFAULT_EVENT_ON_PERCEPTION_CONTINUE ? (1 << EVENT_ON_PERCEPTION_CONTINUE) : 0) |
				(DEFAULT_EVENT_ON_PERCEPTION_END ? (1 << EVENT_ON_PERCEPTION_END) : 0) |
				(DEFAULT_AUTO_PUSH_EVENTS ? (1 << AUTO_PUSH_EVENTS) : 0) |
				(DEFAULT_AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES ? (1 << AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES) : 0)
			),
			mOffset(DEFAULT_OFFSET),
			mRadius(DEFAULT_RADIUS),
			mLineOfSightOffset(DEFAULT_LINE_OF_SIGHT_OFFSET),
			mIgnoreObstaclesSmallerThan(DEFAULT_IGNORE_OBSTACLES_SMALLER_THAN),
			mIgnoredObstaclesAspect(DEFAULT_IGNORED_OBSTACLES_ASPECT),
			mAttachedToSensor(nullptr),
			mCurrentPerceptionsIndex(0),
			mEventId(DEFAULT_EVENT_ID),
			mUpdateInterval(DEFAULT_UPDATE_INTERVAL,DEFAULT_UPDATE_INTERVAL_JITTER),
			mTransformedSensorPosition(0.0f, 0.0f, 0.0f),
			mTransformedLineOfSightStartPosition(0.0f, 0.0f, 0.0f),
			mDebugDrawRequestId(getUninitialized<uint32>())
		{
		}

		inline size_t SensorConfiguration::getNumPerceptions() const
		{
			return mPerceptions[mCurrentPerceptionsIndex].size();
		}

		inline const Perception* SensorConfiguration::getPerception(size_t index) const
		{
			return index<mPerceptions[mCurrentPerceptionsIndex].size() ? &mPerceptions[mCurrentPerceptionsIndex][index] : nullptr;
		}

		inline const std::vector<Perception>& SensorConfiguration::getPerceptions() const
		{
			return mPerceptions[mCurrentPerceptionsIndex];
		}

		inline void SensorConfiguration::addPerception(const Perception& perception)
		{
			mPerceptions[mCurrentPerceptionsIndex].push_back(perception);
		}

		inline void SensorConfiguration::clearPerceptions()
		{
			mPerceptions[0].clear();
			mPerceptions[1].clear();
		}

		inline const Perception* SensorConfiguration::getPerceptionForEntity(uint64 entityId) const
		{
			const auto& perceptions=mPerceptions[mCurrentPerceptionsIndex];
			for (size_t t = 0; t < perceptions.size(); ++t)
			{
				if (perceptions[t].getEntityID() == entityId)
					return &perceptions[t];
			}
			return nullptr;
		}

		inline bool SensorConfiguration::getAutomaticUpdate() const
		{
			return isFlagSet<AUTOMATIC_UPDATE>();
		}

		inline void SensorConfiguration::setAutomaticUpdate(bool automaticUpdate)
		{
			setFlagWithNotification<AUTOMATIC_UPDATE>(automaticUpdate,AUTOMATIC_UPDATE_PROPERTY_ID);
		}

		inline UnsignedFloat SensorConfiguration::getUpdateInterval() const
		{
			return mUpdateInterval.getInterval();
		}

		inline void SensorConfiguration::setUpdateInterval(UnsignedFloat updateInterval)
		{
			if (mUpdateInterval.getInterval()!=updateInterval)
			{
				mUpdateInterval.setInterval(updateInterval);
				notifySensorPropertyChange(UPDATE_INTERVAL_PROPERTY_ID);
			}
		}

		inline UnsignedFloat SensorConfiguration::getUpdateIntervalJitter() const
		{
			return mUpdateInterval.getIntervalJitter();
		}

		inline void SensorConfiguration::setUpdateIntervalJitter(UnsignedFloat updateIntervalJitter)
		{
			if (mUpdateInterval.getIntervalJitter() != updateIntervalJitter)
			{
				mUpdateInterval.setIntervalJitter(updateIntervalJitter);
				notifySensorPropertyChange(UPDATE_INTERVAL_JITTER_PROPERTY_ID);
			}
		}

		inline const glm::vec3& SensorConfiguration::getOffset() const
		{
			return mOffset;
		}

		inline UnsignedFloat SensorConfiguration::getRadius() const
		{
			return mRadius;
		}

		inline bool SensorConfiguration::getInterpreteAspectsAsClassNames() const
		{
			return isFlagSet<INTERPRETE_ASPECTS_AS_CLASS_NAMES>();
		}

		inline void SensorConfiguration::setInterpreteAspectsAsClassNames(bool interpreteAspectsAsClassNames)
		{
			setFlagWithNotification<INTERPRETE_ASPECTS_AS_CLASS_NAMES>(interpreteAspectsAsClassNames,INTERPRETE_ASPECTS_AS_CLASS_NAMES_PROPERTY_ID);
		}

		inline void SensorConfiguration::setRadius(UnsignedFloat radius)
		{
			setAttributeIfChangedAndNotifySensor(mRadius, radius, RADIUS_PROPERTY_ID);
		}

		inline bool SensorConfiguration::getLineOfSightRequired() const
		{
			return isFlagSet<LINE_OF_SIGHT_REQUIRED>();
		}

		inline void SensorConfiguration::setLineOfSightRequired(bool lineOfSightRequired)
		{
			setFlagWithNotification<LINE_OF_SIGHT_REQUIRED>(lineOfSightRequired, LINE_OF_SIGHT_REQUIRED_PROPERTY_ID);
		}

		inline const glm::vec3& SensorConfiguration::getLineOfSightOffset() const
		{
			return mLineOfSightOffset;
		}

		inline UnsignedFloat SensorConfiguration::getIgnoreObstaclesSmallerThan() const
		{
			return mIgnoreObstaclesSmallerThan;
		}

		inline void SensorConfiguration::setIgnoreObstaclesSmallerThan(UnsignedFloat ignoreObstaclesSmallerThan)
		{
			setAttributeIfChangedAndNotifySensor(mIgnoreObstaclesSmallerThan, ignoreObstaclesSmallerThan, IGNORE_OBSTACLES_SMALLER_THAN_PROPERTY_ID);
		}

		inline const AspectTags& SensorConfiguration::getAspect() const
		{
			return mAspect;
		}

		inline const std::string& SensorConfiguration::getAspectTags() const
		{
			return mAspect.getTags();
		}

		inline void SensorConfiguration::setAspectTags(const std::string& tags)
		{
			if (mAspect.getTags() != tags)
			{
				mAspect.setTags(tags);
				notifySensorPropertyChange(ASPECT_TAGS_PROPERTY_ID);
			}
		}

		inline const AspectTags& SensorConfiguration::getIgnoredObstaclesAspect() const
		{
			return mIgnoredObstaclesAspect;
		}

		inline const std::string& SensorConfiguration::getIgnoredObstaclesAspectTags() const
		{
			return mIgnoredObstaclesAspect.getTags();
		}

		inline void SensorConfiguration::setIgnoredObstaclesAspectTags(const std::string& tags)
		{
			if (mIgnoredObstaclesAspect.getTags() != tags)
			{
				mIgnoredObstaclesAspect.setTags(tags);
				notifySensorPropertyChange(IGNORED_OBSTACLES_ASPECT_TAGS_PROPERTY_ID);
			}
		}

		inline const std::string& SensorConfiguration::getEventIdName() const
		{
			return mEventId.getName();
		}

		inline void SensorConfiguration::setEventIdName(const std::string& name)
		{
			if (mEventId.getName() == name)
				return;
			mEventId = NamedIdentifier(name);
			notifySensorPropertyChange(EVENT_ID_PROPERTY_ID);
		}

		inline uint32 SensorConfiguration::getEventId() const
		{
			return mEventId.getHash();
		}

		inline bool SensorConfiguration::getRecordEndingPerceptions() const
		{
			return isFlagSet<RECORD_ENDING_PERCEPTIONS>();
		}

		inline void SensorConfiguration::setRecordEndingPerceptions(bool enable)
		{
			setFlagWithNotification<RECORD_ENDING_PERCEPTIONS>(enable,RECORD_ENDING_PERCEPTIONS_PROPERTY_ID);
		}

		inline bool SensorConfiguration::getEventOnPerceptionBegin() const
		{
			return isFlagSet<EVENT_ON_PERCEPTION_BEGIN>();
		}

		inline void SensorConfiguration::setEventOnPerceptionBegin(bool enable)
		{
			setFlagWithNotification<EVENT_ON_PERCEPTION_BEGIN>(enable, EVENT_ON_PERCEPTION_BEGIN_PROPERTY_ID);
		}

		inline bool SensorConfiguration::getEventOnPerceptionContinue() const
		{
			return isFlagSet<EVENT_ON_PERCEPTION_CONTINUE>();
		}

		inline void SensorConfiguration::setEventOnPerceptionContinue(bool enable)
		{
			setFlagWithNotification<EVENT_ON_PERCEPTION_CONTINUE>(enable, EVENT_ON_PERCEPTION_CONTINUE_PROPERTY_ID);
		}

		inline bool SensorConfiguration::getEventOnPerceptionEnd() const
		{
			return isFlagSet<EVENT_ON_PERCEPTION_END>();
		}

		inline void SensorConfiguration::setEventOnPerceptionEnd(bool enable)
		{
			setFlagWithNotification<EVENT_ON_PERCEPTION_END>(enable, EVENT_ON_PERCEPTION_END_PROPERTY_ID);
		}

		inline bool SensorConfiguration::getAutoPushEvents() const
		{
			return isFlagSet<AUTO_PUSH_EVENTS>();
		}

		inline void SensorConfiguration::setAutoPushEvents(bool enable)
		{
			setFlagWithNotification<AUTO_PUSH_EVENTS>(enable, AUTO_PUSH_EVENTS_PROPERTY_ID);
		}

		inline bool SensorConfiguration::getAutoPushEventsToPerceivedEntities() const
		{
			return isFlagSet<AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES>();
		}

		inline void SensorConfiguration::setAutoPushEventsToPerceivedEntities(bool enable)
		{
			setFlagWithNotification<AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES>(enable, AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES_PROPERTY_ID);
		}

		inline bool SensorConfiguration::fireEventForPerceptionState(PerceptionState state) const
		{
			return (mFlags & (1 << state)) != 0;
		}

		inline void SensorConfiguration::setAttachedToSensor(SensorComponent* attachedToSensor)
		{
			if (mAttachedToSensor!=attachedToSensor)
			{
				mAttachedToSensor=attachedToSensor;
				notifySensorPropertyChange(0);
			}
		}

		inline void SensorConfiguration::resetTriggerTimer()
		{
			mUpdateInterval.resetTimer();
		}

		inline void SensorConfiguration::updateTriggerTimer(float secondsPassed)
		{
			mUpdateInterval.updateTimer(secondsPassed);
		}

		inline bool SensorConfiguration::isItTimeToUpdate() const
		{
			return mUpdateInterval.isItTimeToUpdate();
		}

		inline void SensorConfiguration::cancelDebugDraw() const
		{
			rememberDebugDrawAndCancelPrevious(getUninitialized<uint32>());
		}

		inline bool SensorConfiguration::mayHaveEvents() const
		{
			return isFlagSet<MAY_HAVE_EVENTS>();
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		template<SensorConfiguration::SensorConfigurationFlag E> inline bool SensorConfiguration::isFlagSet() const
		{
			return (mFlags & (1 << E)) != 0;
		}

		template<SensorConfiguration::SensorConfigurationFlag E>  void SensorConfiguration::setFlag(bool enable)
		{
			if (enable)
				mFlags |= (1 << E);
			else
				mFlags &= ~(1 << E);
		}

		template<typename T> inline bool SensorConfiguration::setAttributeIfChangedAndNotifySensor(T& member, T value, uint32 propertyID)
		{
			if (member != value)
			{
				member = value;
				notifySensorPropertyChange(propertyID);
				return true;
			}
			return false;
		}

		template<SensorConfiguration::SensorConfigurationFlag E> inline void SensorConfiguration::setFlagWithNotification(bool enable, uint32 propertyId)
		{
			if (isFlagSet<E>() != enable)
			{
				setFlag<E>(enable);
				notifySensorPropertyChange(propertyId);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
