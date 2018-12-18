// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionRule.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		namespace reactionrules
		{


			#define CLONEABLE_STANDARD_REACTION_RULE(ClassName) \
				virtual ReactionRule* clone() const {return new ClassName(*this);}\
				virtual const char* getRegistrationName() const {return #ClassName ;}


			/** StandardRule is the base class for most of the common concrete
			* reaction-rules you find in this folder.
			* It also provides a semi-automatic interface to name the rule
			* for registration with the reaction-system.
			*
			* Additional JSON configuration tags:
			* EventId
			*/
			class QSF_AI_API_EXPORT StandardRule : public ReactionRule
			{
				// Tag string IDs used by the JSON serializer.
				//@{
				static const char* EVENT_ID_TAG;
				//@}

			public:
				StandardRule(EvaluationParameters::PerceptionEventSource eventSource);

				// Override this method to return the name for this rule.
				// This name is used to register this rule with the ReactionSystem
				// and in turn is the name by which this rule can be instantiated through a JSON configuration.
				virtual const char* getRegistrationName() const = 0;

				// JSON serialization methods.
				// This default implementation serializes mEventId.
				//@{
				virtual void deserialize(const boost::property_tree::ptree& tree);
				virtual void serialize(boost::property_tree::ptree& tree) const;
				//@}

			protected:
				// Returns an event-interator that can be used to loop through all perception events
				// of ID mEventId from the event-source specified by mEventSource.
				EvaluationParameters::PerceptionEventIterator checkForEvent(const EvaluationParameters& parameters) const;

			protected:
				NamedIdentifier mEventId;
				EvaluationParameters::PerceptionEventSource mEventSource;
			};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
		} // reactionrules
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "StandardRule-inl.h"
