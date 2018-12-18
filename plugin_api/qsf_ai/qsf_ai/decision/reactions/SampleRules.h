// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionSystem.h"


namespace sample
{
	namespace perceptionevents
	{
		static const uint32 WAITING_POINT_EVENT=qsf::StringHash("WaitingPoint");
	}

	namespace reactionrules
	{
		/** CheckWaitingPoint is a ReactionRule to be attached to a waiting-point's ReactionComponent.
		* It works in combination with a SensorComponent also attached to that waiting-point.
		* If the sensor detects a potential entity that should wait it is supposed to create a WaitingPoint event.
		* This rule checks the sensor's events and if there is a WaitingPoint-event it
		* - forwards the WaitingPoint-event to the target-entity if there is no "caught" entity yet.
		* - does nothing if the "caught" entity is still visible by the sensor.
		* - returns to "forwarding"-behaviour if the formerly "caught" entity is not seen by the sensor anymore.
		*/
		class QSF_AI_API_EXPORT CheckWaitingPoint : public qsf::ai::ReactionRuleCloneable<CheckWaitingPoint>
		{
		public:
			CheckWaitingPoint();
			virtual void serialize(qsf::BinarySerializer& serializer);
			virtual bool handle(const qsf::ai::EvaluationParameters& parameters);
		private:
			uint64 caught_entity_id;
		};

		/** WaitAtWaitingPoint is a ReactionRule to be attached to an entity's ReactionComponent
		* which is supposed to potentially wait at a waiting-point.
		* If a waiting-point wants this entity to wait it sends that entity a WaitingPoint-event.
		* WaitAtWaitingPoint is designed to react on such an event.
		*/
		class QSF_AI_API_EXPORT WaitAtWaitingPoint : public qsf::ai::ReactionRuleCloneable<WaitAtWaitingPoint>
		{
		public:
			virtual bool handle(const qsf::ai::EvaluationParameters& parameters);
		};

	}
}
