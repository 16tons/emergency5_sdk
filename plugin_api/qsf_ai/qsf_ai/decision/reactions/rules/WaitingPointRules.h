// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/rules/StandardRule.h"
#include "qsf_ai/decision/reactions/ReactionHelpers.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		namespace reactionrules
		{


			/** CheckWaitingPoint is a ReactionRule to be attached to a waiting-point's ReactionComponent.
			* It works in combination with a SensorComponent also attached to that waiting-point.
			* If the sensor detects a potential entity that should wait it is supposed to create a WaitingPoint event.
			* This rule checks the sensor's events and if there is a WaitingPoint-event it
			* - forwards the WaitingPoint-event to the target-entity if there is no "caught" entity yet.
			* - does nothing if the "caught" entity is still visible by the sensor.
			* - returns to "forwarding"-behaviour if the formerly "caught" entity is not seen by the sensor anymore.
			*
			* Sensor-Configuration:
			* The corresponding sensor has to be configured to
			* AutoPushEvents=true
			* EventsOnPerceptionBegin=true
			* EventsOnPerceptionContinue=false
			* EventsOnPerceptionEnd=true
			*/
			class QSF_AI_API_EXPORT CheckWaitingPoint : public StandardRule
			{
			public:
				CLONEABLE_STANDARD_REACTION_RULE(CheckWaitingPoint)
				CheckWaitingPoint();
				virtual void serialize(BinarySerializer& serializer) override;
				virtual bool handle(const EvaluationParameters& parameters) override;
			protected:
				uint64 mCaughtEntityId;
			};


			/** WaitAtWaitingPoint is a ReactionRule to be attached to an entity's ReactionComponent
			* which is supposed to potentially wait at a waiting-point.
			* If a waiting-point wants this entity to wait it sends that entity a WaitingPoint-event.
			* WaitAtWaitingPoint is designed to react on such an event.
			* The method onWait is called in that case. The default implementation simply lets the entity
			* wait for the duration specified.
			*
			* Sensor-Configuration:
			* no sensor required.
			*
			* Additional JSON configuration tags:
			* MinDuration, MaxDuration
			*/
			class QSF_AI_API_EXPORT WaitAtWaitingPoint : public StandardRule
			{
			public:
				CLONEABLE_STANDARD_REACTION_RULE(WaitAtWaitingPoint)
				WaitAtWaitingPoint();
				virtual void deserialize(const boost::property_tree::ptree& tree);
				virtual void serialize(boost::property_tree::ptree& tree) const;
				virtual bool handle(const EvaluationParameters& parameters);
			protected:
				virtual bool onWait(const EvaluationParameters& parameters,const PerceptionEvent& event);
			protected:
				MinMaxDuration mDuration;
			};


			/** WaitAtWaitingPointTurnAndAnimate specializes WaitAtWaitingPoint
			* by providing functionality to automatically let the waiting entity
			* face the waiting-point and play an animation.
			*
			* Sensor-Configuration:
			* no sensor required.
			*
			* Additional JSON configuration tags:
			* MinDuration, MaxDuration, Animation, TurnReverse
			*/
			class QSF_AI_API_EXPORT WaitAtWaitingPointTurnAndAnimate : public WaitAtWaitingPoint
			{
			public:
				CLONEABLE_STANDARD_REACTION_RULE(WaitAtWaitingPointTurnAndAnimate)
				WaitAtWaitingPointTurnAndAnimate();
				virtual void deserialize(const boost::property_tree::ptree& tree);
				virtual void serialize(boost::property_tree::ptree& tree) const;
			protected:
				virtual bool onWait(const EvaluationParameters& parameters,const PerceptionEvent& event);
			protected:
				Animation mAnimation;
				TurnToPerceiver mTurnToPerceiver;
			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // reactionrules
	} // ai
} // qsf
