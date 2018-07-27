// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/SensorConfiguration.h"

#include <qsf/reflection/type/CampQsfAssetProxy.h>
#include <qsf/logic/EffortIndicator.h>

#include <boost/ptr_container/ptr_vector.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class PerceptionDebugGroup;
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
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    The base class for all sensor-components.
		*    Currently two types of sensors exist, derived from this class:
		*
		*    SingleSensorComponent:
		*    A sensor comprised out of one single sensor-configuration,
		*    easily configurable from inside the editor.
		*
		*    MultipleSensorsComponent:
		*    A sensor comprised out of any number of sensor-configurations.
		*    This allows to have any number of sensors attached to one entity.
		*    Configuration is done via a JSON asset.
		*/
		class QSF_AI_API_EXPORT SensorComponent : public Component
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SensorComponent(Prototype *prototype);

			// Overridden Component interface
			//@{
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			// Get the summed number of perceptions (summed because it may be a MultipleSensorComponent)
			// TODO(dm) mSummedNumPerceptions is only adjusted by the process method for now.
			// If you manually remove sensors / manually add or clear perceptions etc. this value is not updated!
			size_t getSummedNumPerceptions() const;

			// Returns a perception by index (where index is inside the range 0 >= index < getSummedNumPerceptions).
			// If the index is out of range a nullptr is returned.
			// If you don't need info on the sensor's configuration simply pass a nullptr into sensorConfiguration.
			virtual const Perception* getPerceptionByIndex(size_t index,const SensorConfiguration** sensorConfiguration) const=0;

			// Check if a concrete entity is currently perceived by this sensor.
			// If this is true then the respective Perception is returned, as well as the sensor-configuration that caused the detection (optional).
			// If this is false then a nullptr is returned and the optional sensorConfiguration-object is not modified.
			// If you don't need info on the sensor's configuration simply pass a nullptr into sensorConfiguration.
			virtual const Perception* isPerceivingEntity(uint64 entityId,const SensorConfiguration** sensorConfiguration) const=0;

			// Issues an update-cycle on all configurations of that sensor.
			// If jobArguments is 0 then an update of all configurations is enforced.
			// Otherwise the jobArgument's time information is used to trigger updates only if necessary.
			// Configurations marked as non-automatic are not triggered at all in that case.
			// This method is called automatically by the SensorPerceptionSystem.
			effort::Indicator update(const JobArguments* jobArguments);

			// Same as update, but only updates the sensor-configuration(s) that match the given event-id.
			// If eventId is 0 then this method behaves exactly like the update method above.
			virtual effort::Indicator updateForEvent(uint32 eventId,const JobArguments* jobArguments)=0;

			// Generates events depending on the event-settings and the current perceptions and pushs them to
			// a ReactionComponent (If reactionComponent is a nullptr then the ReactionComponent of the entity the sensor is attached to is used)
			// or a PerceptionEventQueue.
			// If eventId is 0, then all available events are pushed.
			// Otherwise only the events of the type specified are pushed.
			// Those methods are especially useful if the sensor's configuration(s) are not using the auto-push-events feature.
			// Then the event-pushing can be triggered manually by calling those methods.
			// This essentially means: instead of letting the sensor actively push events as soon as they happen,
			// you can transfer this work to the event-listener and actively poll the events there when you need them.
			//@{
			void pushEvents(uint32 eventId,ReactionComponent* reactionComponent) const;
			virtual void pushEvents(uint32 eventId,PerceptionEventQueue& perceptionEventQueue) const=0;
			//@}

			// Returns true if there is the chance that there are events available.
			bool getMayHaveEvents() const;

			// Dummy methods with the sole purpose of triggering a sensor-update
			// from inside the editor by simple clicking on the ForceUpdateFromEditor property.
			// For testing only. Getter always returns false.
			//@{
			void setForceUpdateFromEditor(bool force);
			bool getForceUpdateFromEditor() const;
			//@}

			// Creates the actual debug output for this component.
			// This method is called automatically by the SensorPerceptionSystem.
			virtual void createDebugOutput(const PerceptionDebugGroup& debugSettings)=0;

			// Called by the attached sensor-configurations setters if there's any property change.
			void notifySensorPropertyChange(uint32 propertyId);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			// Loops through all configurations, updates the configuration timers and
			// triggers the real updates of the respective configuration, if necessary.
			// Called by the update method.
			effort::Indicator process(SensorConfiguration*const* sensorConfigurations, size_t numSensorConfigurations, const ProcessingParameters& params);

			// records property change to do pre-computations later (not used as of now)
			virtual void onComponentPropertyChange(const Component& component, uint32 propertyId);

			// Methods to draw the sensor inside the editor.
			//@{
			virtual void onSetDebug(bool debug); // to indirectly catch on-editor-select notification
			virtual void drawSensor(bool show) = 0;
			//@}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			// Method to perform eventual static precalculations on configuration changes.
			// Called internally if the sensor's configurations have changed.
			void recomputeInternals(SensorConfiguration*const* configurations,size_t numConfigurations);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			bool mHasToRecomputeInternals; // flag to signal property changes in any of the sensor's configurations
			size_t mSummedNumPerceptions; // sum of all perceptions
			bool mMayHaveEvents; // flag to signal the potential availability of events


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
#include "qsf_ai/perception/SensorComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::SensorComponent);
