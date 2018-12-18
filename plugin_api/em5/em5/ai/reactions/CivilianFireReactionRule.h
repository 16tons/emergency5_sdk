// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf_ai/decision/reactions/rules/StandardRule.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	/** Reaction rule for fire danger
	* Just lets sensed entities (civilians) run away from fire
	*/
	class CivilianFireReactionRule : public qsf::ai::reactionrules::StandardRule
	{
	public:
		CLONEABLE_STANDARD_REACTION_RULE(em5::CivilianFireReactionRule);
		CivilianFireReactionRule();

		virtual bool handle(const qsf::ai::EvaluationParameters& parameters);
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
