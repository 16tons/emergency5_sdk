// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/base/Percentage.h"
#include "qsf_ai/decision/reactions/EvaluationParameters.h"

#include <qsf/script/Script.h>
#include <qsf/asset/AssetProxy.h>
#include <qsf/base/NamedIdentifier.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/** ReactionRule is the base class for concrete rules for use with ReactionComponents.
		* Its purpose is to provide an interface for
		* evaluating a situation (usually by checking PerceptionEvents)
		* and, if that evaluation was positive, for
		* triggering a corresponding activity (usually by adding Actions to an ActionComponent).
		*
		* To implement a concrete rule simply
		* - derive from ReactionRule (or better ReactionRuleCloneable)
		* - implement the handle-method
		* - inside that check if there are events of interest to you
		* - if not simply return "false"
		* - return "true" in case you actually triggered an Action
		*
		* To actually use that rule inside the qsf-system you have to
		* register it with the ReactionSystem.
		* You should do so at game startup:
		* reactionSystem->registerReactionRule("MyReactionRule",new MyReactionRule());
		*
		* Now your rule can be referenced by its name inside the
		* rule-configuration-asset-JSONs which in turn is used to configure ReactionComponents.
		*/
		class QSF_AI_API_EXPORT ReactionRule
		{
			friend class ReactionSystem;
			friend class ReactionRulesTableAdder;
		public:
			ReactionRule();
			ReactionRule(const ReactionRule& src);
			virtual ~ReactionRule();

			virtual ReactionRule* clone() const=0;

			// Read-only access to the ReactionRule's id.
			//@{
			const std::string& getName() const;
			uint32 getId() const; // string hash of the ReactionRule's name.
			//@}

			// Access the rule's probability
			//@{
			Percentage getProbability() const;
			void setProbability(Percentage probability);
			//@}

			// Access the rule's priority
			//@{
			action::Priority getPriority() const;
			//@}

			// The core method of a ReactionRule.
			// It has to contain the code you need to
			// decide whether the rule's conditions are met (e.g. by checking the parameters for certain PerceptionEvents)
			// and to trigger any action.
			// Return "true" if you trigger any action and want to stop checking other rules with lower or equal priority.
			virtual bool handle(const EvaluationParameters& parameters)=0;

			// Return true to force rule-handling even if the ReactionRulesTable would not execute it
			// due to the current action being of higher or equal priority.
			// Default implementation returns false.
			virtual bool wantsHandlingDespiteOfLowerPriority() const;

			// Binary (de)serialize a rule. You have to override this method in case your rule stores any data.
			// The default implementation is empty.
			virtual void serialize(BinarySerializer& serializer);

			// Methods to (de)serialize this rule from / to a JSON.
			// Besides the common attributes (name, priority, probability) every rule
			// may define its own additional configuration parameters.
			// The default implementations are empty.
			//@{
			virtual void deserialize(const boost::property_tree::ptree& tree);
			virtual void serialize(boost::property_tree::ptree& tree) const;
			//@}

			// used for std::sort. Returns true if a has a higher priority than b
			static bool compareByPriority(const ReactionRule* a,const ReactionRule* b);

		protected:
			// Method that allows forwarding events to other entities.
			// This is particulary useful in the following scenario:
			// - attractor's sensor scans for other entities that may be attracted.
			// - attractor's reaction-rule checks if such an event "entity entered attractor's area" took place.
			// - attractor's reaction-rule then forwards the event to the actual entity attracted by calling pushEventToEntity.
			// That way you don't have to attach a attractor-sensor to every entity around, but let the attractor itself do the
			// detection.
			// Returns false if the target entity cannot be found or if the target entity has no ReactionComponent attached.
			bool pushEventToEntity(const PerceptionEvent& perceptionEvent) const;

		private:
			// only accessable to the ReactionSystem, which is a friend of this class for that purpose.
			void setName(const char* name);
			// only accessable to the ReactionRuleAdder which is a friend of this class for that purpose.
			void setPriority(action::Priority priority);
		private:
			NamedIdentifier mRuleId; // the rule's name / ID. The name is used to reference it inside reaction-table-configuration-JSONs.
			Percentage mProbability; // the rule's probability. The ReactionComponent triggers this rule's handle-method only if a probility vs. random-number check is positive.
			action::Priority mPriority; // the rule's priority. The reaction-rules-table handles its rules ordered by highest to lowest priority.
		};


		/** Helper class to enforce creation of the ReactionRule's clone-method.
		*/
		template<typename Derived> class ReactionRuleCloneable : public ReactionRule
		{
		public:
			virtual ReactionRule* clone() const
			{
				return new Derived(static_cast<Derived const &>(*this));
			}
		};

		/** Helper class to create ReactionRules based on scripts or small
		* C++ modules, called "Handlers".
		*/
		template<typename Derived,typename ConditionHandler,typename ReactionHandler>
			class ReactionRuleWithHandlers : public ReactionRuleCloneable<Derived>
		{
		typedef ReactionRuleWithHandlers<Derived,ConditionHandler,ReactionHandler> ThisType;
		public:
			inline ReactionRuleWithHandlers(const ConditionHandler& conditionHandler,const ReactionHandler& reactionHandler) :
				ReactionRuleCloneable<Derived>(),
				mConditionHandler(conditionHandler),
				mReactionHandler(reactionHandler)
			{
			}

			inline ReactionRuleWithHandlers(const ThisType& src) :
				ReactionRuleCloneable<Derived>(src),
				mConditionHandler(src.mConditionHandler),
				mReactionHandler(src.mReactionHandler)
			{
			}

		protected:
			ConditionHandler mConditionHandler;
			ReactionHandler mReactionHandler;
		};


		class ReactionRuleHandlerEmpty
		{
		public:
			inline bool execute(const PerceptionEvent& parameters)
			{
				return true;
			}
		};


		class ReactionRuleHandlerScript
		{
		public:
			ReactionRuleHandlerScript(const AssetProxy& scriptAsset);
			ReactionRuleHandlerScript(const ReactionRuleHandlerScript& src);
			~ReactionRuleHandlerScript();
			bool execute(const PerceptionEvent& parameters);
		private:
			const AssetProxy mScriptAsset;
			Script* mScriptInstance;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionRule-inl.h"
