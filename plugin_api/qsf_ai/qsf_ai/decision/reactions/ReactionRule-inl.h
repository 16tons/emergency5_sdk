// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{


		// inline methods ReactionRule
		inline ReactionRule::ReactionRule() :
			mPriority(0)
		{
		}

		inline ReactionRule::ReactionRule(const ReactionRule& src)  :
			mRuleId(src.mRuleId),
			mProbability(src.mProbability),
			mPriority(src.mPriority)
		{
		}

		inline ReactionRule::~ReactionRule()
		{
		}

		inline void ReactionRule::setName(const char* name)
		{
			mRuleId=name;
		}

		inline const std::string& ReactionRule::getName() const
		{
			return mRuleId.getName();
		}

		inline uint32 ReactionRule::getId() const
		{
			return mRuleId.getHash();
		}

		inline Percentage ReactionRule::getProbability() const
		{
			return mProbability;
		}

		inline void ReactionRule::setProbability(Percentage probability)
		{
			mProbability=probability;
		}

		inline action::Priority ReactionRule::getPriority() const
		{
			return mPriority;
		}

		inline void ReactionRule::setPriority(action::Priority priority)
		{
			mPriority=priority;
		}

		inline bool ReactionRule::wantsHandlingDespiteOfLowerPriority() const
		{
			return false;
		}

		inline void ReactionRule::serialize(BinarySerializer& serializer)
		{
		}

		inline void ReactionRule::deserialize(const boost::property_tree::ptree& tree)
		{
		}

		inline void ReactionRule::serialize(boost::property_tree::ptree& tree) const
		{
		}

		inline bool ReactionRule::compareByPriority(const ReactionRule* a,const ReactionRule* b)
		{
			return a->getPriority()>b->getPriority();
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
