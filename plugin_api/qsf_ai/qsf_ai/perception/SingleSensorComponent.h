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
		*    A sensor component with one single sensor-configuration.
		*    Entities equipped with such a sensor gain the ability to detect other entities
		*    with a matching aspect-component / matching components attached.
		*/
		class QSF_AI_API_EXPORT SingleSensorComponent : public SensorConfiguration, public SensorComponent
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			// unique CAMP ids for the component and for properties
			//@{
			QSF_DECLARE_CAMP_COMPONENT_ID;
			//@}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			SingleSensorComponent(Prototype* prototype);

			// Overridden Component interface
			//@{
			virtual void serialize(BinarySerializer& serializer) override;
			//@}

			virtual effort::Indicator updateForEvent(uint32 eventId, const JobArguments* jobArguments);

			virtual void pushEvents(uint32 eventId, PerceptionEventQueue& perceptionEventQueue) const;

			virtual const Perception* getPerceptionByIndex(size_t index, const SensorConfiguration** sensorConfiguration) const;
			virtual const Perception* isPerceivingEntity(uint64 entityId, const SensorConfiguration** sensorConfiguration) const;

			virtual void createDebugOutput(const PerceptionDebugGroup& debugSettings);


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		protected:
			virtual void drawSensor(bool show);

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
#include "qsf_ai/perception/SingleSensorComponent-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::SingleSensorComponent);
