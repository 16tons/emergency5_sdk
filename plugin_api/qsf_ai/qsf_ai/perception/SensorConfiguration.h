// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/AspectComponent.h"
#include "qsf_ai/perception/PerceptionEvent.h"
#include "qsf_ai/base/UpdateInterval.h"

#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
class btDynamicsWorld;
namespace qsf
{
	class Entity;
	class JobArguments;
	class TransformComponent;
	class PhysicsWorldComponent;
	namespace ai
	{
		class SensorComponent;
		class PerceptionDebugGroup;
		class ReactionComponent;
		class PerceptionEventQueue;
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		//[-------------------------------------------------------]
		//[ Helping Classes                                       ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    processing-parameters are a convenient helper for supplying all necessary
		*    fixed parameters for a sensor-update-run in one place.
		*    It automatically extracts, validates and cashs additional information from the
		*    entity whose sensor is to be updated.
		*/
		class ProcessingParameters
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			ProcessingParameters(const Entity& perceiver,const JobArguments* jobArguments,uint32 eventId);

		//[-------------------------------------------------------]
		//[ Public data                                           ]
		//[-------------------------------------------------------]
		public:
			const Entity& mPerceiver; // the perceiving entity, the entity whose sensor is to be updated.
			const TransformComponent* mPerceiverTransform; // the perceiving entity's transformation
			PhysicsWorldComponent* mPhysicsWorldComponent; // the physics world component the perceiving entity belongs to
			const JobArguments* mJobArguments; // the job-arguments the caller supplied (can be nullptr in case of a forced update)
			const uint32 mEventId; // 0 if all configuration shall be updated, otherwise only the configuration that matches this event-id is processed.
		};


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    A sensor-configuration contains all necessary information to configure
		*    a sensor or a part of a multiple-sensor.
		*    It defines a sensor's position and range relative to the entity the
		*    corresponding sensor is attached to.
		*    It holds an aspect-tags object to configure what kind of entities the
		*    sensor should detect.
		*    The aspect-tags can either be used to scan for entities with a matching
		*    AspectComponent or for entities equipped with certain components by
		*    interpreting the aspect-tags as component class names.
		*    It also allows to configure an optional line-of-sight-check, so that only
		*    non-occluded entities are actually perceived (while also allowing to
		*    "look through" certain obstacles).
		*    A sensor-configuration also holds the perceptions resulting from an update.
		*/
		class QSF_AI_API_EXPORT SensorConfiguration
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// Tag string IDs to be used with the JSON serializer
			// to load and save a sensor-configuration.
			//@{
			static const char* SENSOR_CONFIGURATION_TAG;
			static const char* AUTOMATIC_UPDATE_TAG;
			static const char* OFFSET_TAG;
			static const char* RADIUS_TAG;
			static const char* ASPECT_TAGS_TAG;
			static const char* INTERPRETE_ASPECTS_AS_CLASS_NAMES_TAG;
			static const char* LINE_OF_SIGHT_REQUIRED_TAG;
			static const char* LINE_OF_SIGHT_OFFSET_TAG;
			static const char* IGNORE_OBSTACLES_SMALLER_THAN_TAG;
			static const char* IGNORED_OBSTACLES_ASPECT_TAGS_TAG;
			static const char* EVENT_ID_TAG;
			static const char* RECORD_ENDING_PERCEPTIONS_TAG;
			static const char* EVENT_ON_PERCEPTION_BEGIN_TAG;
			static const char* EVENT_ON_PERCEPTION_CONTINUE_TAG;
			static const char* EVENT_ON_PERCEPTION_END_TAG;
			static const char* AUTO_PUSH_EVENTS_TAG;
			static const char* AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES_TAG;
			//@}

			// default values for the properties, also used by the CAMP system.
			//@{
			static const bool DEFAULT_AUTOMATIC_UPDATE;
			static const UnsignedFloat DEFAULT_UPDATE_INTERVAL;
			static const UnsignedFloat DEFAULT_UPDATE_INTERVAL_JITTER;
			static const glm::vec3 DEFAULT_OFFSET;
			static const UnsignedFloat DEFAULT_RADIUS;
			static const bool DEFAULT_INTERPRETE_ASPECTS_AS_CLASS_NAMES;
			static const bool DEFAULT_LINE_OF_SIGHT_REQUIRED;
			static const glm::vec3 DEFAULT_LINE_OF_SIGHT_OFFSET;
			static const UnsignedFloat DEFAULT_IGNORE_OBSTACLES_SMALLER_THAN;
			static const std::string DEFAULT_IGNORED_OBSTACLES_ASPECT;
			static const std::string DEFAULT_EVENT_ID;
			static const bool DEFAULT_RECORD_ENDING_PERCEPTIONS;
			static const bool DEFAULT_EVENT_ON_PERCEPTION_BEGIN;
			static const bool DEFAULT_EVENT_ON_PERCEPTION_CONTINUE;
			static const bool DEFAULT_EVENT_ON_PERCEPTION_END;
			static const bool DEFAULT_AUTO_PUSH_EVENTS;
			static const bool DEFAULT_AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES;
			//@}

			// unique property IDs for the CAMP system.
			//@{
			static const unsigned int AUTOMATIC_UPDATE_PROPERTY_ID;
			static const unsigned int UPDATE_INTERVAL_PROPERTY_ID;
			static const unsigned int UPDATE_INTERVAL_JITTER_PROPERTY_ID;
			static const unsigned int OFFSET_PROPERTY_ID;
			static const unsigned int RADIUS_PROPERTY_ID;
			static const unsigned int ASPECT_TAGS_PROPERTY_ID;
			static const unsigned int INTERPRETE_ASPECTS_AS_CLASS_NAMES_PROPERTY_ID;
			static const unsigned int LINE_OF_SIGHT_REQUIRED_PROPERTY_ID;
			static const unsigned int LINE_OF_SIGHT_OFFSET_PROPERTY_ID;
			static const unsigned int IGNORE_OBSTACLES_SMALLER_THAN_PROPERTY_ID;
			static const unsigned int IGNORED_OBSTACLES_ASPECT_TAGS_PROPERTY_ID;
			static const unsigned int EVENT_ID_PROPERTY_ID;
			static const unsigned int PERCEPTIONS_PROPERTY_ID;
			static const unsigned int RECORD_ENDING_PERCEPTIONS_PROPERTY_ID;
			static const unsigned int EVENT_ON_PERCEPTION_BEGIN_PROPERTY_ID;
			static const unsigned int EVENT_ON_PERCEPTION_CONTINUE_PROPERTY_ID;
			static const unsigned int EVENT_ON_PERCEPTION_END_PROPERTY_ID;
			static const unsigned int AUTO_PUSH_EVENTS_PROPERTY_ID;
			static const unsigned int AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES_PROPERTY_ID;
			//@}

		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SensorConfiguration();
			virtual ~SensorConfiguration();

			// Methods to (de)serialize this sensor-configuration.
			//@{
			void deserializeFromAssetTree(const boost::property_tree::ptree& tree);
			void serializeToAssetTree(boost::property_tree::ptree& tree) const;
			//@}

			// Binary serialization.
			// Most of the configuration is supposed to be static and is not serialized,
			// please refer to the implementation to check which attributes are actually serialized.
			// The current perceptions are and timer states are definitely serialized.
			// Called by the respective component's serialize-method
			void serialize(BinarySerializer& serializer);

			// The core method that does the actual work of detecting
			// which entities can be "seen" by an entity using a sensor
			// with this sensor-configuration.
			void updatePerceptions(const ProcessingParameters& params);

			// Access to the perceived entities.
			//@{
			size_t getNumPerceptions() const;
			const Perception* getPerception(size_t index) const; // with bounds-check, return nullptr if index is out of range
			const std::vector<Perception>& getPerceptions() const;
			void addPerception(const Perception& perception);
			void clearPerceptions();
			const Perception* getPerceptionForEntity(uint64 entityId) const;
			//@}

			// Creates the actual debug output for this configuration.
			// Internally called by the corresponding SensorComponent, triggered by the SensorPerceptionSystem.
			void createConfigurationDebugOutput(const PerceptionDebugGroup& debugSettings,Entity& perceiver,size_t numPerceptions,bool isLastConfig) const;

			// Draws the sensor's graphical representation.
			// Used to show it inside the editor.
			void drawSensorConfiguration(bool show,const Entity& perceiver) const;

			// Shall a sensor that uses this configuration update its perceivers
			// automatically in a certain interval, triggered by the perception-system?
			// Or shall it be handled manually and update its list of perceived entities
			// on demand (AutomaticUpdate=false)?
			//@{
			bool getAutomaticUpdate() const;
			void setAutomaticUpdate(bool automaticUpdate);
			//@}

			// The (automatic) update interval in seconds.
			//@{
			UnsignedFloat getUpdateInterval() const;
			void setUpdateInterval(UnsignedFloat updateInterval);
			//@}

			// Defines an optional random jitter value that is added to the
			// update-trigger-timer when resetting it to the update-interval.
			//@{
			UnsignedFloat getUpdateIntervalJitter() const;
			void setUpdateIntervalJitter(UnsignedFloat updateInterval);
			//@}

			// Offset of the sensor relative to the perceiving entity's position and direction.
			// Rotates and scales with the entity.
			//@{
			const glm::vec3& getOffset() const;
			void setOffset(const glm::vec3& offset);
			//@}

			// Sensor's radius defining the size of the spherical area in which the detection of other
			// entities is done.
			//@{
			UnsignedFloat getRadius() const;
			void setRadius(UnsignedFloat radius);
			//@}

			// Toggle whether the aspect-tags are interpreted as
			// strings for detecting entities equipped with an AspectComponent (default) or
			// if they are interpreted as CAMP class names for detecting entities equipped with certain components.
			//@{
			bool getInterpreteAspectsAsClassNames() const;
			void setInterpreteAspectsAsClassNames(bool interpreteAspectsAsClassNames);
			//@}

			// Toggle whether a line-of-sight-check is required.
			// If set to true then for each perceived entity a ray-test is performed.
			// If there is an obstacle in between the perceiver and the entity then
			// the entity is discarded from the set of perceptions.
			//@{
			bool getLineOfSightRequired() const;
			void setLineOfSightRequired(bool lineOfSightRequired);
			//@}

			// Offset to the start-position of the ray cast when doing a line-of-sight-check.
			// Relative to the perceiver, NOT relative to the sensor's offset!
			//@{
			const glm::vec3& getLineOfSightOffset() const;
			void setLineOfSightOffset(const glm::vec3& lineOfSightOffset);
			//@}

			// If an obstacle is detected during the optional line-of-sight-check
			// its rough extends are compared to that value. If the obstacle is
			// smaller then the line-of-sight-check ignores that obstacle and continues
			// as if there was no obstacle at all.
			// Set to 0.0 (default) to turn off that test.
			//@{
			UnsignedFloat getIgnoreObstaclesSmallerThan() const;
			void setIgnoreObstaclesSmallerThan(UnsignedFloat ignoreObstaclesSmallerThan);
			//@}

			// The list of aspects the sensor should react on.
			// Only entities with an AspectTags-component and at least one matching aspect-tag
			// are detected.
			// If there are no aspect-tags defined the sensor will detect every entity with an
			// AspectTags-component.
			//@{
			const std::string& getAspectTags() const;
			void setAspectTags(const std::string& tags);
			const AspectTags& getAspect() const;
			//@}

			// If an obstacle is detected during the optional line-of-sight-check
			// it's aspect-tags (if it has an AspectTags-component attached) is
			// compared against this aspect-tags-list. If there's a match then
			// the obstacle is ignored and the line-of-sight-check continues as if
			// there was no obstacle at all.
			//@{
			const std::string& getIgnoredObstaclesAspectTags() const;
			void setIgnoredObstaclesAspectTags(const std::string& tags);
			const AspectTags& getIgnoredObstaclesAspect() const;
			//@}

			// Access to the configuration's event-id.
			// For everything a sensor senses depending on its configuration
			// it does not just manage a perception list
			// but can also automatically feed PerceptionEvents into a ReactionComponent.
			//@{
			const std::string& getEventIdName() const;
			void setEventIdName(const std::string& name);
			uint32 getEventId() const; // string hash of the event's name.
			//@}

			// Toggle whether entities that were perceived during the former update
			// are still recorded inside the perceptions-array (with the state "perception ended")
			// when they exit the sensor's perception-area during this update.
			//@{
			bool getRecordEndingPerceptions() const;
			void setRecordEndingPerceptions(bool enable);
			//@}

			// Toggle which kind of events are generated by the sensor.
			//@{
			bool getEventOnPerceptionBegin() const;
			void setEventOnPerceptionBegin(bool enable);
			bool getEventOnPerceptionContinue() const;
			void setEventOnPerceptionContinue(bool enable);
			bool getEventOnPerceptionEnd() const;
			void setEventOnPerceptionEnd(bool enable);
			//@}

			// Toggle whether events are automatically pushed to a ReactionComponent attached to this entity
			// whenever the sensor is updated.
			//@{
			bool getAutoPushEvents() const;
			void setAutoPushEvents(bool enable);
			//@}

			// Toggle whether events are automatically pushed to the perceived entities (if the respective entity has a ReactionComponent attached)
			// whenever the sensor is updated.
			//@{
			bool getAutoPushEventsToPerceivedEntities() const;
			void setAutoPushEventsToPerceivedEntities(bool enable);
			//@}

			// Generates events depending on the event-settings and the current perceptions
			// and pushs them to a PerceptionEventQueue or a ReactionComponent.
			// Note: if AutoPushEvents is enabled then this method is called automatically during sensor-update to
			// immediately feed a ReactionComponent attached to the perceiving entity.
			//@{
			void pushEventsTo(PerceptionEventQueue& eventQueue,uint64 perceivingEntityId) const;
			void pushEventsTo(ReactionComponent& reactionComponent,uint64 perceivingEntityId) const;
			//@}

			// Generates events depending on the event-settings and the current perceptions
			// and pushs them to the respective perceived entities' ReactionComponents.
			// Note: if AutoPushEventsToPerceivedEntities is enabled then this method is called automatically during sensor-update to
			// immediately forward the events to the perceived entities' ReactionComponents.
			//@{
			void pushEventsToPerceivedEntities(uint64 perceivingEntityId) const;
			//@}

			// Attach this configuration to a sensor-component.
			// This is used internally to notify that sensor of property changes
			// inside this configuration.
			void setAttachedToSensor(SensorComponent* attachedToSensor);

			// Updates the internal update-countdown and returns true if the update-interval countdown has reached <= 0.
			// In that case the internal counter is resetted to the update-interval's value.
			// Used internally by the SensorComponent to trigger automatic updates.
			//@{
			void updateTriggerTimer(float secondsPassed);
			bool isItTimeToUpdate() const;
			void resetTriggerTimer();
			//@}

			// Hint indicating whether there are any events. Used internally for speedup.
			bool mayHaveEvents() const;


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			SensorConfiguration(const SensorConfiguration& source);
			SensorConfiguration& operator=(const SensorConfiguration& source);

			// mFlags can be any combination of those flags.
			//@{
			enum SensorConfigurationFlag {
				// order matters. Do not touch!
				//@{
				EVENT_ON_PERCEPTION_BEGIN=0,
				EVENT_ON_PERCEPTION_CONTINUE,
				EVENT_ON_PERCEPTION_END,
				//@}

				AUTOMATIC_UPDATE,
				LINE_OF_SIGHT_REQUIRED,
				INTERPRETE_ASPECTS_AS_CLASS_NAMES,
				RECORD_ENDING_PERCEPTIONS,

				AUTO_PUSH_EVENTS,
				AUTO_PUSH_EVENTS_TO_PERCEIVED_ENTITIES,

				USES_OFFSET, // internal flag to determine whether the sensor's center has to be transformed. This is true if mOffset is not (0,0,0)
				USES_LINE_OF_SIGHT_OFFSET, // internal flag to determine whether the sensor's line-of-sight-position has to be transformed. This is true if mLineOfSightOffset is not (0,0,0)
				MAY_HAVE_EVENTS, // internal flag to early quit during before event-creation / pushing in case there are neither current nor previous perceptions
			};
			//@}

			// Internally used helper methods to simplify mFlags-access.
			//@{
			template<SensorConfigurationFlag E> bool isFlagSet() const;
			template<SensorConfigurationFlag E> void setFlag(bool enable);
			bool fireEventForPerceptionState(PerceptionState state) const;
			//@}

			// Used internally by the setters to notify an attached sensor
			// that a property was changed.
			// TODO(dm) this will be used to trigger recomputation of various
			// internals for optimization purposes.
			//@{
			void notifySensorPropertyChange(uint32 propertyId);
			template<typename T> bool setAttributeIfChangedAndNotifySensor(T& member,T value,uint32 propertyID);
			template<SensorConfigurationFlag E> void setFlagWithNotification(bool enable,uint32 propertyId);
			//@}

			// Internally used helper methods to simplify debug-drawing.
			// Note: those are only necessary due to the existence of mDebugDrawRequestId (see its comment).
			void rememberDebugDrawAndCancelPrevious(uint32 debugDrawRequestId) const;
			void cancelDebugDraw() const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32 mFlags;
			glm::vec3 mOffset;
			UnsignedFloat mRadius;
			glm::vec3 mLineOfSightOffset;
			UnsignedFloat mIgnoreObstaclesSmallerThan;
			AspectTags mAspect;
			AspectTags mIgnoredObstaclesAspect;
			SensorComponent* mAttachedToSensor;

			// The mPerceptions are "double-buffered" internally, mPerceptions[mCurrentPerceptionsIndex] contains the latest perceptions, mPerceptions[1-mCurrentPerceptionsIndex] the perceptions of the former update.
			// This is done to easily / efficiently determine the PerceptionState of each new perception.
			unsigned int mCurrentPerceptionsIndex;
			std::vector<Perception> mPerceptions[2]; // result data - contains ids and additional info of perceived entities. Also see mCurrentPerceptionsIndex.

			NamedIdentifier mEventId; // for convenience the event ID is a string at design time

			UpdateInterval mUpdateInterval;  // manages remaining time until the next automatic update, the interval and an optional jitter

			glm::vec3 mTransformedSensorPosition;
			glm::vec3 mTransformedLineOfSightStartPosition;
			std::vector<Perception> mOccludedPerceptions; // contains the potential perceptions that were removed during lines-of-sight-check. Used for debugging only at the moment, therefore not accessable from outside.

			// Debug draw request ID, "qsf::getUninitialized<uint32>()" if invalid.
			// Since the only reason for its existence is due to a bug inside the editor that
			// hinders one-frame-debug-outputs from vanishing, it is made mutable to not
			// remove const-ness from otherwise const methods.
			// TODO(dm) remove this attribute and all associated methods as soon as said problem with the editor is gone.
			mutable uint32 mDebugDrawRequestId;


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI();


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/SensorConfiguration-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::SensorConfiguration);
