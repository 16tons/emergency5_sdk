// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	// Central point to register the reactions and reaction-rules available.
	// If you define new reactions or reaction-rules add those to the respective
	// functions you find inside the corresponding cpp-unit.
	// Note that the actual registration may be delayed until the
	// ReactionSystem is actually instantiated.
	void installReactionsAndReactionRules();


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
