// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_ai/Export.h"


// Contains setting how the pathfinding my correct positions in certain situations.
// The typical problems are being outside the defined AI map boundaries or inside a permanently blocked area.
// In all cases if a non blocked area is as close as a blocked one, this will always be preferred.
namespace qsf
{
	namespace ai
	{
		namespace position
		{
			// The options how to correct a position under certain circumstances.
			class QSF_AI_API_EXPORT CorrectionOptions
			{
			public:
				static const bool DEFAULT_CORRECT_ILLEGAL_POSITION_SETTING;
				static const bool DEFAULT_CORRECT_BLOCKED_POSITION_SETTING;
				static const bool DEFAULT_CORRECT_TO_IDEAL_POSITION_SETTING;

				// Used the static default settings and corrects a very liberal setting which always tries to correct a state but not to the ideal position
				CorrectionOptions();

				// This flag governs whether an illegal position is corrected to the closest legal position or not.
				// Whether a legal position includes a blocked state depends on the flag correctBlocked below.
				bool mCorrectIllegalState;

				// This flag governs whether we treat a permanently blocked state as illegal and correct it.
				// Only permanently blocked states are considered here, temporary blocks should by their nature always be considered soon to be free again.
				bool mCorrectPermanentBlockedState;

				// This flag governs where inside an area the position is actually corrected to.
				// If set to false, any position inside an area may be used.
				// For illegal corrected positions this means the beeline closest position inside the area is selected which is often very close to the border.
				// If set to true, a corrected position is moved further into the area so that it is at an ideal spot for traversing the area.
				// This also takes effect if the position is not illegal.
				// The exact nature of an ideal position depends on the native shapes for the areas.
				// It might be the midpoint for a triangle or a square and any point on the connection between the two defining nodes of a trapezoid for the lane world for example.
				bool mCorrectToIdealPosition;
			};


			// Tiny struct that contains all cases for position errors according to a single search
			class CorrectionSettings
			{
			public:
				// Initializes the options with the default constructor
				CorrectionSettings();

				CorrectionOptions mStartSettings; // The correction options to apply for the start state
				CorrectionOptions mGoalSettings; // The correction options to apply for the goal states
			};
		}
	}
}

#include "qsf_ai/navigation/pathfinding/PositionCorrection-inl.h"
