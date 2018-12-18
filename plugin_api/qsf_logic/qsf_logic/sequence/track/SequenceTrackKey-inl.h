// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Public static methods                                 ]
		//[-------------------------------------------------------]
		inline bool SequenceTrackKey::compareKeyTimes(const SequenceTrackKey* a, const SequenceTrackKey* b)
		{
			return (a->mFrameNumber < b->mFrameNumber);
		}

		inline bool SequenceTrackKey::compareKeyWithTime(const SequenceTrackKey* a, const uint32& b)
		{
			return (a->mFrameNumber < b);
		}

		inline bool SequenceTrackKey::compareTimeWithKey(const uint32& a, const SequenceTrackKey* b)
		{
			return (a < b->mFrameNumber);
		}


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline SequenceTrackKey::SequenceTrackKey(uint32 frameNumber) :
			mFrameNumber(frameNumber)
		{
			// Nothing to do in here
		}

		inline uint32 SequenceTrackKey::getFrameNumber() const
		{
			return mFrameNumber;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
