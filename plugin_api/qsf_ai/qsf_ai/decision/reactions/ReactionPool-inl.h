// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		inline ReactionPool::ReactionPool() :
			mProbabilityRange(0.0f)
		{
		}

		inline ReactionPool::~ReactionPool()
		{
			clear();
		}

		inline void ReactionPool::clear()
		{
			for (size_t t=0;t<mReactions.size();++t)
			{
				delete mReactions[t];
			}
			mReactions.clear();
			mProbabilityRange=0.0f;
		}


		inline ReactionPoolAdder::ReactionPoolAdder(ReactionPool& reactionPool) :
			mReactionPool(reactionPool)
		{
			mReactionPool.clear();
		}

		inline void ReactionPoolAdder::addReaction(Reaction* reaction)
		{
			if (reaction)
				mReactions.push_back(reaction);
		}

		inline void ReactionPoolAdder::addReaction(Reaction* reaction,float probability)
		{
			if (reaction)
			{
				reaction->setProbability(probability);
				mReactions.push_back(reaction);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
