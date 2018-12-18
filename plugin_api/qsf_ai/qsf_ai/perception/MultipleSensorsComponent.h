// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/SensorComponent.h"


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
		*    A sensor component with any number of sensor-configurations.
		*    Entities equipped with such a sensor gain the ability to detect other entities
		*    with a matching aspect-component / matching components attached.
		*    A MultipleSensorsComponent has to be used in cases where one sensor is not enough
		*    and cannot be handled by one sensor with multiple aspect-tags.
		*/
		class QSF_AI_API_EXPORT MultipleSensorsComponent : public SensorComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// unique CAMP ids for the component and for properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			static const unsigned int SENSOR_CONFIGURATIONS_ASSET_PROPERTY_ID;
			//@}

			static const std::string DEFAULT_SENSOR_CONFIGURATIONS_ASSET;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			MultipleSensorsComponent(Prototype* prototype);

			// Overridden Component interface
			//@{
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			virtual effort::Indicator updateForEvent(uint32 eventId,const JobArguments* jobArguments);

			virtual void pushEvents(uint32 eventId,PerceptionEventQueue& perceptionEventQueue) const;

			virtual const Perception* getPerceptionByIndex(size_t index,const SensorConfiguration** sensorConfiguration) const;
			virtual const Perception* isPerceivingEntity(uint64 entityId,const SensorConfiguration** sensorConfiguration) const; // if more than one sensorConfiguration detected the target entity then the first perception / sensor-configuration is returned.

			virtual void createDebugOutput(const PerceptionDebugGroup& debugSettings);

			// Access to the senor configurations.
			// The setter is empty, NumberOfSensorConfigurations was simply added
			// to provide a read-only property in the editor to
			// allow for a quick visual check if the number of configurations is
			// the one you'd expect.
			//@{
			const boost::ptr_vector<SensorConfiguration>& getSensorConfigurations() const;
			uint32 getNumberOfSensorConfigurations() const;
			inline void setNumberOfSensorConfigurations(uint32 dummy) {}
			//@}

			// Get or set the asset that contains the sensor's configurations.
			// The internal mSensorConfigurations-array is rebuilt if necessary.
			//@{
			const AssetProxy& getSensorConfigurationsAsset() const;
			void setSensorConfigurationsAsset(const AssetProxy& asset);
			//@}

		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void drawSensor(bool show);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			boost::ptr_vector<SensorConfiguration> mSensorConfigurations; // array containing the deserialized sensor configurations.
			AssetProxy mSensorConfigurationsAsset; // Asset containing the serialized sensor configurations.

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
#include "qsf_ai/perception/MultipleSensorsComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::MultipleSensorsComponent);
