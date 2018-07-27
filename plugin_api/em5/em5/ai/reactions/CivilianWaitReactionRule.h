// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/ai/reactions/CivilianReactions.h"

#include <qsf_ai/decision/reactions/rules/WaitingPointRules.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/** Reaction rule for waiting at a waiting-point.
	* Just lets perceived entities (civilians) wait and turn to the waiting-point.
	*/
	class CivilianWaitReactionRule : public qsf::ai::reactionrules::WaitAtWaitingPoint
	{
	public:
		CLONEABLE_STANDARD_REACTION_RULE(em5::CivilianWaitReactionRule);
		CivilianWaitReactionRule();

	protected:
		virtual bool onWait(const qsf::ai::EvaluationParameters& parameters, const qsf::ai::PerceptionEvent& event);
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
