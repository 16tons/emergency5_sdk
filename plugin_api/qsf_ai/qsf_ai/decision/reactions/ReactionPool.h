// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/Reaction.h"
#include "qsf_ai/base/UsuallySmallArray.h"

#include <vector>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		/** A ReactionPool is a collection of Reactions.
		* It is useful in situations where a ReactionRule's
		* action is not fixed but one out of many.
		* Each Reaction is assigned a probablity the
		* ReactionPool uses to randomly select one from its list.
		*/
		class QSF_AI_API_EXPORT ReactionPool
		{
			friend class ReactionPoolAdder;
			// Tag string IDs used by the JSON serializer.
			//@{
			static const char* REACTION_POOL_TAG;
			static const char* REACTION_NAME_TAG;
			//@}
			ReactionPool& operator=(const ReactionPool& src); // not implemented
		public:
			ReactionPool();
			ReactionPool(const ReactionPool& src);
			~ReactionPool();

			void serialize(BinarySerializer& serializer);
			void deserialize(const boost::property_tree::ptree& tree);
			void serialize(boost::property_tree::ptree& tree) const;

			void clear();

			// Returns a random reaction from the pool, depending on the reactions' probabilities.
			// So reactions with higher probability are more likely to be returned than those with lower probability.
			Reaction* selectReaction() const;

			// Returns a random reaction from the pool, depending on the reactions' probabilities.
			// So reactions with higher probability are more likely to be returned than those with lower probability.
			// The reaction that's being returned is automatically executed, passing the EvaluationParameters and the event provided.
			// If no reaction is chosen then the process is repeated up to numTries times.
			Reaction* executeReaction(const EvaluationParameters& parameters,const PerceptionEvent* event,uint32 numTries) const;
		private:
			void sortReactions();
		private:
			float mProbabilityRange;
			UsuallySmallArray<Reaction*,size_t,5> mReactions;
		};


		/** Helper class to manually add Reactions to a ReactionPool.
		* It takes care that all internals of the ReactionPool are
		* set up correctly and that all methods are called in the
		* correct order.
		* Use it in case you want to manually fill a ReactionPool
		* with reactions.
		*/
		class QSF_AI_API_EXPORT ReactionPoolAdder
		{
		public:
			ReactionPoolAdder(ReactionPool& reactionPool);
			~ReactionPoolAdder();
			void addReaction(Reaction* reaction);
			void addReaction(Reaction* reaction,float probability);
		private:
			ReactionPool& mReactionPool;
			std::vector<Reaction*> mReactions;
		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_ai/decision/reactions/ReactionPool-inl.h"
