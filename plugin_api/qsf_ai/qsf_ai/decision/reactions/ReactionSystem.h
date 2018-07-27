// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"
#include "qsf_ai/base/StandardSystem.h"
#include "qsf_ai/decision/reactions/ReactionComponent.h"

#include <qsf/math/RandomCreator.h>
#include <qsf/reflection/CampDefines.h>
#include <qsf/component/Component.h>

#include <unordered_map>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{
		class ReactionDebugGroup;
		class Reaction;
		namespace reactionrules
		{
			class StandardRule;
		}
	}
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/** AI system to execute reactions for entities that have a reaction component attached.
		* Noncopyable by inheritance.
		* Besides updating the ReactionComponents it also holds a list of all registered ReactionRules.
		* Registered rules can be referenced by name from ReactionRule-assets.
		* Use sf::ai::ReactionSystem::registerReactionRule to register a rule.
		* Those registrations should be placed inside the function qsf::ai::registerReactionRules
		* which has to be defined somewhere in your code (for the sample it is inside TestRules.cpp)
		*/
		class QSF_AI_API_EXPORT ReactionSystem : public StandardSystem<ReactionComponent, ReactionSystem>
		{
		public:
			QSF_DECLARE_CAMP_COMPONENT_ID;

			// Plain text identifier
			static const char* NAME;

			ReactionSystem(Prototype* prototype);
			virtual ~ReactionSystem();

			// public System implementation override
			//@{
			virtual bool onStartup() override;
			//@}

			// required ai::System template interface implementation
			//@{
			effort::Indicator updateComponent(ReactionComponent& rc, const JobArguments& arguments);
			void createDebugOutput(const ReactionComponent& rc) const;
			void updateGlobals(const JobArguments&) const;
			//@}

			// Add a PerceptionEvent to the ReactionSystem's global event-queue.
			// ReactionComponents that are interested in those events can get them
			// during their next update cycle.
			// The global event queue is cleared after an update-cycle.
			void pushEvent(const PerceptionEvent& perceptionEvent);

			// Add a PerceptionEvent to the ReactionComponent's local event-queue of the targetEntity.
			bool pushEventToLocalQueueOfEntity(const PerceptionEvent& perceptionEvent,const Entity* targetEntity) const;

			// Access the global perception event-queue
			PerceptionEventQueue& getPerceptionEventQueue() const;

			// Register a ReactionRule and assign it an unique ID.
			// If registered a ReactionRule can be referenced and instantiated via assets for use with ReactionComponents.
			// Should be called from inside your implementation of registerReactionRules.
			//@{
			void registerReactionRule(const char* name,ReactionRule* reactionRule);
			void registerReactionRule(reactionrules::StandardRule* reactionRule);
			//@}

			// Register a Reaction and assign it an unique ID.
			// If registered a Reaction can be referenced and instantiated via assets for use with ReactionPools.
			// Should be called from inside your implementation of registerReactions.
			//@{
			void registerReaction(Reaction* reaction);
			//@}

			// Create a concrete reaction-rule by cloning a registered one.
			// Internally used by the ReactionTableSerializer to build a table of ReactionRules
			// from a JSON containing rule-names.
			// If a ReactionRule with that name is not found (= not registered) a nullptr is returned
			// and a QSF_ERROR is shown.
			ReactionRule* createReactionRuleByName(const std::string& name) const;

			// Create a concrete reaction by cloning a registered one.
			// Internally used by the ReactionPoolSerializer to build a table of ReactionPool
			// from a JSON containing reaction-names.
			// If a Reaction with that name is not found (= not registered) a nullptr is returned
			// and a QSF_ERROR is shown.
			Reaction* createReactionByName(const std::string& name) const;

			// Returns the reaction-system's debug-group or a nullptr if there is no debug-group.
			ReactionDebugGroup* getDebugSettings() const;

			// You have to call this function to install your register-functions into the reaction-system.
			// This is usually done on the app's startup.
			//@{
			typedef void (*RegisterFunction)(ReactionSystem&);
			static void installRegisterFunctions(RegisterFunction registerReactions,RegisterFunction registerRules);
			//@}

			// Managing reaction rules tables
			void getReactionRulesTable(ReactionRulesTable& reactionTable, const AssetProxy& reactionRulesAsset) const;

		protected:
			// overridden StandardComponent interface
			//@{
			virtual void updateSystem(const JobArguments& jobArguments) override;
			//@}
		private:
			PerceptionEventQueue* mGlobalPerceptionEventQueue; // the global perception queue, handled by the ReactionSystem itself.
			ReactionDebugGroup* mDebugSettings; // pointer to the debug settings, may be null meaning no debugging output should be generated.
			std::unordered_map<uint32,ReactionRule*> mReactionRules; // The registered reaction-rules.
			std::unordered_map<uint32,Reaction*> mReactions; // The registered reactions.
			static RegisterFunction mRegisterReactionsFunc;
			static RegisterFunction mRegisterReactionRulesFunc;

			mutable std::unordered_map<uint64, ReactionRulesTable> mReactionRulesTables;

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
#include "qsf_ai/decision/reactions/ReactionSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::ai::ReactionSystem);
