// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/serialization/binary/BasicTypeSerialization.h>

namespace qsf
{
	namespace ai
	{
		namespace position
		{
			inline CorrectionOptions::CorrectionOptions() :
				mCorrectIllegalState(DEFAULT_CORRECT_ILLEGAL_POSITION_SETTING),
				mCorrectPermanentBlockedState(DEFAULT_CORRECT_BLOCKED_POSITION_SETTING),
				mCorrectToIdealPosition(DEFAULT_CORRECT_TO_IDEAL_POSITION_SETTING)
			{}

			inline CorrectionSettings::CorrectionSettings()
			{}
		}
	}

	namespace serialization
	{
		// specializations for using the QSF binary serializer
		template <>
		struct serializer<ai::position::CorrectionOptions>
		{
			inline static void serialize(BinarySerializer& serializer, ai::position::CorrectionOptions& options)
			{
				serializer & options.mCorrectIllegalState;
				serializer & options.mCorrectPermanentBlockedState;
				serializer & options.mCorrectToIdealPosition;
			}
		};

		template <>
		struct serializer<ai::position::CorrectionSettings>
		{
			inline static void serialize(BinarySerializer& serializer, ai::position::CorrectionSettings& settings)
			{
				serializer & settings.mStartSettings;
				serializer & settings.mGoalSettings;
			}
		};
	}
}
