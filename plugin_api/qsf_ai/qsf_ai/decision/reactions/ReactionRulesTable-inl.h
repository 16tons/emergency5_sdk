// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/perception/PerceptionEventQueue.h"
#include "qsf/platform/PlatformSystem.h"
#include "qsf/QsfHelper.h"


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace ai
	{

		inline ReactionRulesTable::ReactionRulesTable() :
			mRandomRealCreate(0.0f, 1.0, static_cast<uint32>(QSF_PLATFORM.getSystemTime().getMicroseconds()))
		{
			//Nothing to do
		}

		inline ReactionRulesTable::ReactionRulesTable(const ReactionRulesTable& reactionRulesTable) :
			mRandomRealCreate(0.0f, 1.0, static_cast<uint32>(QSF_PLATFORM.getSystemTime().getMicroseconds()))
		{
			operator=(reactionRulesTable);
		}

		// inline methods ReactionRulesTable
		inline ReactionRulesTable::~ReactionRulesTable()
		{
			clear();
		}

		inline const ReactionRulesTable::Contents& ReactionRulesTable::getRules() const
		{
			return mContents;
		}

		inline void ReactionRulesTable::clear()
		{
			for (size_t t=0;t<mContents.size();++t)
			{
				delete mContents[t];
			}
			mContents.clear();
		}

		inline size_t ReactionRulesTable::getNumRules() const
		{
			return mContents.size();
		}


		// inline methods ReactionRulesTableAdder
		inline ReactionRulesTableAdder::ReactionRulesTableAdder(ReactionRulesTable& reactionRulesTable) :
			mReactionRulesTable(reactionRulesTable)
		{
		}

		inline void ReactionRulesTableAdder::addReactionRule(action::Priority priority,ReactionRule* rule)
		{
			if (rule)
			{
				rule->setPriority(priority);
				mReactionRules.push_back(rule);
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // ai
} // qsf
