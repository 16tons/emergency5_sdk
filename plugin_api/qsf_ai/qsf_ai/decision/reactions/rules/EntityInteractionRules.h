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
#include "qsf_ai/base/RingBuffer.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		namespace reactionrules
		{


			/** Base class for entity-interaction (for example social-behaviour).
			* It handles notification of the respective social participant
			* by forwarding the event to it.
			* A derived class has to override the isValidParticipant-method
			* for any additional checks beforehand
			* (for example: is it a civilian, is it going by foot, etc.).
			* To actually trigger an action onBeginInteration has to be overridden.
			* onEndInteration is called when the participant left.
			*
			* Sensor-Configuration:
			* The corresponding sensor has to be configured to
			*
			* AutoPushEvents=true
			* EventOnPerceptionStart=true
			* EventOnPerceptionContinue=false
			* EventOnPerceptionEnd=false
			*
			* Note: the participant (!) entity doesn't strictly need a sensor.
			* (example: prostitute vs. male civilian behaviour, only the prostitute needs a sensor)
			*/
			class QSF_AI_API_EXPORT EntityInteraction : public StandardRule
			{
			public:
				EntityInteraction(bool expectAcknowledgeFromParticipant);
				virtual void serialize(BinarySerializer& serializer) override;
				virtual bool handle(const EvaluationParameters& parameters) override;
				virtual bool wantsHandlingDespiteOfLowerPriority() const;

			protected:
				enum InteractionStatus
				{
					NO_INTERACTION=0,
					INTERACTION_STARTED,
					STILL_INTERACTING,
					INTERACTION_ENDED
				};
				InteractionStatus checkAndHandleEvents(const EvaluationParameters& parameters,PerceptionEvent& event);
				bool triggerCallbacks(InteractionStatus status,const EvaluationParameters& parameters,PerceptionEvent& event);
				void sendCancelEventTo(const EvaluationParameters& parameters,uint64 targetEntityId) const;
				bool cancelInteraction(const EvaluationParameters& parameters,bool notifyParticipant);
				void updatePreviousInteractionsRecords(const EvaluationParameters& parameters);
				bool hasBeenInteractionWith(uint64 entityId) const;
				void recordInteraction(const EvaluationParameters& parameters);
				Entity* getParticipant() const;
				virtual bool isValidParticipant(const EvaluationParameters& parameters,const PerceptionEvent& event,Entity& entity);
				virtual bool onBeginInteration(const EvaluationParameters& parameters,const PerceptionEvent& event)=0;
				virtual bool onStillInterating(const EvaluationParameters& parameters,const PerceptionEvent& event);
				virtual void onEndInteration(const EvaluationParameters& parameters,const PerceptionEvent& event);

			private:
				bool mExpectAcknowledgeFromParticipant;
				uint64 mWaitingForAcknowledgeFrom;
				uint64 mPariticipantEntityId;

			protected:
				float mNoInteractionWithSameParticipantFor;
				struct PreviousInteraction
				{
					uint64 mPariticipantEntityId;
					Time mTimeStamp;
					inline void serialize(BinarySerializer& serializer)
					{
						serializer & mPariticipantEntityId;
						serializer & mTimeStamp;
					}
				};
				RingBuffer<PreviousInteraction,size_t,4> mPreviousInteractions; // records the last 4 interactions
			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // reactionrules
	} // ai
} // qsf
