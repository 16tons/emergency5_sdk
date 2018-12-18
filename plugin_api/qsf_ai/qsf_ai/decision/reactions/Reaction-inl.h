// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		inline Reaction::Reaction() :
			mProbability(1.0f)
		{
		}

		inline Reaction::~Reaction()
		{
		}

		inline void Reaction::setName(const char* name)
		{
			mReactionId=name;
		}

		inline const std::string& Reaction::getName() const
		{
			return mReactionId.getName();
		}

		inline uint32 Reaction::getId() const
		{
			return mReactionId.getHash();
		}

		inline void Reaction::serialize(BinarySerializer& serializer)
		{
		}

		inline float Reaction::getProbability() const
		{
			return mProbability;
		}

		inline void Reaction::setProbability(float probability)
		{
			mProbability=probability;
		}

		inline bool Reaction::compareByProbability(const Reaction* a,const Reaction* b)
		{
			return a->mProbability>b->mProbability;
		}


		inline DelayedReaction::DelayedReaction() :
			Reaction()
		{
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
