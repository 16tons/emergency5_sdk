// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Export.h"

#include <boost/function.hpp>

#include <deque>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Filtered random number generation helper class
	*
	*  @remarks
	*    Random numbers are filtered for psychologically perceived nonrandom pattern.
	*    Different filter can be applied to exclude different pattern.
	*
	*  @note
	*    - Use this class carefully, different filter can take performance depending on their parameter
	*/
	class QSF_API_EXPORT FilteredRandom
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] prototype
		*    The prototype this component is in, no null pointer allowed
		*/
		FilteredRandom();

		/**
		*  @brief
		*    Repeating the same number (1111)
		*
		*  @param[in] enable
		*    Enable or disable the filter
		*/
		void setMaximumNumberOfTries(uint32 maximalNumberOfTries);

		/**
		*  @brief
		*    Use custom random generator
		*/
		void useCustomRandomGenerator(bool enabled, boost::function<uint32(uint32,uint32)> customRandomUintGenerator);

		/**
		*  @brief
		*    Repeating the same number (1111)
		*
		*  @param[in] enable
		*    Enable or disable the filter
		*  @param[in] TODO(tl): Finish me
		*/
		void enableRepeatFilter(bool enable, uint32 repeatLimit = 1);

		/**
		*  @brief
		*    Missing  in a sequence (543422354353, 1)
		*
		*  @param[in] enable
		*    Enable or disable the filter
		*  @param[in] TODO(tl): Finish me
		*/
		void enableMissingFilter(bool enable, uint32 missingInSequence);

		/**
		*  @brief
		*    Appearing to many times in a sequence (1321214131)
		*
		*  @param[in] enable
		*    Enable or disable the filter
		*  @param[in] TODO(tl): Finish me
		*/
		void enableAppearFilter(bool enable, uint32 appearInSequence = 2, uint32 appearLimit = 1);

		/**
		*  @brief
		*    Linear increasing or decreasing sequence for to long (123456, 9876543)
		*
		*  @param[in] enable
		*    Enable or disable the filter
		*  @param[in] TODO(tl): Finish me
		*/
		void enableLinearSequenceFilter(bool enable, uint32 linearSequenceLimit = 2);

		/**
		*  @brief
		*    Repeat duplicates to many times (112233)
		*
		*  @param[in] enable
		*    Enable or disable the filter
		*  @param[in] TODO(tl): Finish me
		*/
		void enableRepeatDuplicatesFilter(bool enable, uint32 repeatDuplicatesLength = 2, uint32 repeatDuplicatesLimit = 1);

		/**
		*  @brief
		*    Repeat pattern (123123123)
		*
		*  @param[in] enable
		*    Enable or disable the filter
		*  @param[in] TODO(tl): Finish me
		*/
		void enableRepeatPatternFilter(bool enable, uint32 repeatPatternLength = 2, uint32 repeatPatternLimit = 1);

		/**
		*  @brief
		*    Repeat pattern (12612412712)
		*
		*  @param[in] enable
		*    Enable or disable the filter
		*  @param[in] TODO(tl): Finish me
		*/
		void enableAppearPatternFilter(bool enable, uint32 appearPatternLength = 2, uint32 appearPatternInSequence = 2, uint32 appearPatternLimit = 1);

		/**
		*  @brief
		*    Get UInt32 random numbers similar as in qsf::Random.h with filtered rules
		*/
		uint32 getRandomUint(uint32 minimum, uint32 maximum);

		// TODO(tl): Filtering not implemented yet for signed integer, floating points and chances
		// int32 getRandomInt(int32 minimum, int32 maximum);
		// float getRandomFloat(float minimum, float maximum);
		// static bool getRandomChance(float chance);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		uint32 tryGetRandomUint(uint32 minimum, uint32 maximum, uint32 tries);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// TODO(tl): Make flags later
		bool mRepeatFilterEnabled;
		bool mMissingFilterEnabled;
		bool mAppearFilterEnabled;
		bool mLinearSequenceFilterEnabled;
		bool mRepeatDuplicatesFilterEnabled;
		bool mRepeatPatternFilterEnabled;
		bool mAppearPatternFilterEnabled;

		uint32 mRepeatLimit;
		uint32 mMissingInSequence;
		uint32 mAppearInSequence;
		uint32 mAppearLimit;
		uint32 mLinearSequenceLimit;
		uint32 mRepeatDuplicatesLength;
		uint32 mRepeatDuplicatesLimit;
		uint32 mRepeatPatternLength;
		uint32 mRepeatPatternLimit;
		uint32 mAppearPatternLength;
		uint32 mAppearPatternInSequence;
		uint32 mAppearPatternLimit;

		uint32 mMaximalMemorySize;
		uint32 mMaximalNumberOfTries;

		std::deque<uint32> mRandomMemory;

		bool mCustomUintRandomGeneratorEnabled;
		boost::function<uint32(uint32,uint32)> mCustomUintRandomGenerator;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
