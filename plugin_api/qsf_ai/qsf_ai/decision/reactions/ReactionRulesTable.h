// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/decision/reactions/ReactionRule.h"
#include "qsf_ai/base/UsuallySmallArray.h"

#include <qsf/logic/EffortIndicator.h>
#include <qsf/logic/action/ActionPriority.h>
#include <qsf/math/RandomCreator.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		class PerceptionEventQueue;
		class PerceptionEvent;


		/** A reaction table defines the rules for reactive behavior.
		* It is made up of a prioritized list of conditions to test and behavior to execute when the condition is evaluated to true.
		* Only behavior with a priority higher than the currently executed one is tested at all and the highest such rule is actually executed.
		*/
		class QSF_AI_API_EXPORT ReactionRulesTable
		{
			friend class ReactionRulesTableAdder;
		private:

			typedef UsuallySmallArray<ReactionRule*,size_t,8> Contents;
		public:
			// Constructor
			inline ReactionRulesTable();

			// Copy Constructor
			inline ReactionRulesTable(const ReactionRulesTable& reactionRulesTable);

			// Destructor
			inline ~ReactionRulesTable();

			// Copy assignment operator
			ReactionRulesTable& operator=(const ReactionRulesTable& reactionRulesTable);

			// Main function that evaluates the rules and executes the reaction with the highest priority whose condition evaluates to true
			// if the priority is higher than the currently executed behavior.
			// To support AI LOD a min. priority can be supplied, while 0 means that every rule is considered for evaluation.
			// @return effort::REGULAR if there was at least one rule tested, otherwise effort::TRIVIAL
			effort::Indicator update(action::Priority minPriority,EvaluationParameters& parameters) const;

			// Deletes all rules from this table.
			inline void clear();

			// Get the number of reaction-rules this table contains.
			inline size_t getNumRules() const;

			// read-only access to the rules (for serialization primarily)
			inline const Contents& getRules() const;

			// Serialize the ReactionTable with all its rules.
			// This is necessary because rules are allowed to have memory.
			void serialize(BinarySerializer& serializer);

		private:
			Contents mContents; // array of the actual ReactionRules, sorted by priority (higher priority first)
			mutable RandomRealCreator<float> mRandomRealCreate; // Create a float random number between 0.0f and 1.0f
		};

		/** Helper class to manually add Rules to a ReactionRulesTable.
		* It takes care that all internals of the ReactionRulesTable are
		* set up correctly and that all methods are called in the
		* correct order.
		* Use it in case you want to manually fill a ReactionRulesTable
		* with rules.
		*/
		class QSF_AI_API_EXPORT ReactionRulesTableAdder
		{
		public:
			inline ReactionRulesTableAdder(ReactionRulesTable& reactionRulesTable);
			~ReactionRulesTableAdder();
			void addReactionRule(action::Priority priority,ReactionRule* rule);
		private:
			ReactionRulesTable& mReactionRulesTable;
			std::vector<ReactionRule*> mReactionRules;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionRulesTable-inl.h"
