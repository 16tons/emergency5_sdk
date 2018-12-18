// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/rules/StandardRule.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		namespace reactionrules
		{


			/** General notes regarding local events:
			*
			* The most obvious way of defining a reaction to a local event is
			* to attach a sensor- and a reaction-component for that event to an entity.
			* This approach is fine if there are many sources for that event
			* (for example: detect and react on burning objects if 50% of the map is burning).
			*
			* But often the "inverse" strategy is more appropriate:
			* if there aren't that many sources for that event it is more efficient
			* to attach a sensor to detect potential REACT-entities to the event-source-entity.
			* That sensor is checked by a pseudo-reaction-rule on that source-entity
			* that then pushs actual events to the target-entities in range.
			* That way you only need 1 sensor for all potential targets; that sensor only
			* needs to be updated / checked if the source-entity decides that this is
			* actually necessary.
			*
			* The explosion of an entity would be one example. The entity knows best when / if
			* it is exploding, so instead of letting all other entities scan the map for
			* explosions all the time, better let the exploding entity scan for potential
			* observers nearby and notify those.
			*
			* The following classes are designed to be used with the latter case.
			*/


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]

			/** ForwardLocalEventRule is a ReactionRule to be attached to a event-source-entity's ReactionComponent.
			* It works in combination with a SensorComponent also attached to that entity.
			* The sensor's purpose is to detect entities that will potentially react on the event.
			* If the rule decides that the local event is happening, it checks the sensor for potential targets
			* and notifies those.
			*
			* Implement fireLocalEvent and return true if your concrete event condition is satisfied.
			* In turn this class will notify all potential target entities.
			*
			* Sensor-Configuration:
			* The corresponding sensor has to be configured to match the
			* constructor's sensorAutoPush parameter
			*
			* sensorAutoPush = true:
			* The sensor is expected to update its perceptions in the sensor-interval specified.
			* Use this mode if your event is triggered pretty often (e.g. entity constantly burning).
			*
			* sensorAutoPush = false:
			* The sensor is updated by the rule itself if fireLocalEvent returns true.
			* Use this mode if your event is seldomly triggered (e.g. entity explodes).
			*/
			class QSF_AI_API_EXPORT ForwardLocalEventRule : public StandardRule
			{
			public:
				ForwardLocalEventRule(bool sensorAutoPush);
				virtual bool handle(const EvaluationParameters& parameters);

			protected:
				virtual bool fireLocalEvent(const EvaluationParameters& parameters) = 0;
			};


			/** HandleForwardedLocalEventRule is the counterpart to ForwardLocalEventRule.
			* It is attached to the entity that's supposed to react on a local event.
			* Implement onLocalEvent to trigger actions on the event. Return true
			* if you actually execute any action.
			*
			* Sensor-Configuration:
			* no sensor required, events are expected to be sent by a ForwardLocalEventRule
			* and to arrive in the local event-queue.
			*/
			class QSF_AI_API_EXPORT HandleForwardedLocalEventRule : public StandardRule
			{
			public:
				HandleForwardedLocalEventRule();
				virtual bool handle(const EvaluationParameters& parameters);

			protected:
				virtual bool onLocalEvent(const EvaluationParameters& parameters, const PerceptionEvent& event) = 0;
			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // reactionrules
	} // ai
} // qsf
