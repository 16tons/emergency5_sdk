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


			//[-------------------------------------------------------]
			//[ Classes                                               ]
			//[-------------------------------------------------------]
			/** Base class for simple reactions on global events
			* A global event in this context here is expected to consist out of one event,
			* a single global event that triggers an action.
			* Derived classes should override onEvent
			* and implement the respective actions there.
			*
			* Sensor-Configuration:
			* No sensor required.
			*/
			class QSF_AI_API_EXPORT GlobalEvent : public StandardRule
			{
			public:
				CLONEABLE_STANDARD_REACTION_RULE(GlobalEvent)
				GlobalEvent();
				virtual bool handle(const EvaluationParameters& parameters);

			protected:
				virtual bool onEvent(const EvaluationParameters& parameters,const PerceptionEvent& event);
			};


			/** Base class for reactions on global events
			* actually consisting out of two events,
			* a start-event and an end-event, with dedicated IDs.
			* Derived classes should override onEventStart and onEventEnd
			* and implement the respective actions there.
			* onEventEnd is only called if onEventStart returnd true.
			*
			* Sensor-Configuration:
			* No sensor required.
			*
			* Additional JSON configuration tags:
			* EndEventId
			*/
			class QSF_AI_API_EXPORT GlobalEventStartEnd : public StandardRule
			{
				// Tag string IDs used by the JSON serializer.
				//@{
				static const char* END_EVENT_ID_TAG;
				//@}
			public:
				CLONEABLE_STANDARD_REACTION_RULE(GlobalEventStartEnd)
				GlobalEventStartEnd();
				virtual void serialize(BinarySerializer& serializer);
				virtual void deserialize(const boost::property_tree::ptree& tree);
				virtual void serialize(boost::property_tree::ptree& tree) const;
				virtual bool handle(const EvaluationParameters& parameters);
			protected:
				virtual bool onEventStart(const EvaluationParameters& parameters,const PerceptionEvent& event);
				virtual void onEventEnd(const EvaluationParameters& parameters,const PerceptionEvent& event);
			private:
				bool mStarted;
				NamedIdentifier mEndEventId;
			};


			class QSF_AI_API_EXPORT GlobalEventStartEndAnimated : public GlobalEventStartEnd
			{
			public:
				CLONEABLE_STANDARD_REACTION_RULE(GlobalEventStartEndAnimated)
				GlobalEventStartEndAnimated();
				virtual void deserialize(const boost::property_tree::ptree& tree);
				virtual void serialize(boost::property_tree::ptree& tree) const;
			protected:
				virtual bool onEventStart(const EvaluationParameters& parameters,const PerceptionEvent& event);
				virtual void onEventEnd(const EvaluationParameters& parameters,const PerceptionEvent& event);
			private:
				Animation mAnimation;
			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // reactionrules
	} // ai
} // qsf
