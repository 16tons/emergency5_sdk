// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/error/BoostAssertHandler.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		inline SequenceTrack::~SequenceTrack()
		{
			// Nothing to do in here
		}

		inline Sequence& SequenceTrack::getSequence() const
		{
			// It's safe in here to cast the pointer to a reference because we know that the pointer is never ever a null pointer
			QSF_ASSERT(nullptr != mSequence, "The owner sequence instance is invalid", QSF_REACT_THROW);
			return *mSequence;
		}

		inline uint32 SequenceTrack::getId() const
		{
			return mId;
		}

		inline bool SequenceTrack::isActive() const
		{
			return mActive;
		}

		inline void SequenceTrack::setActive(bool active)
		{
			mActive = active;
		}

		inline const std::string& SequenceTrack::getName() const
		{
			return mName;
		}

		inline void SequenceTrack::setName(const std::string& name)
		{
			mName = name;
		}

		inline const std::string& SequenceTrack::getDescription() const
		{
			return mDescription;
		}

		inline void SequenceTrack::setDescription(const std::string& description)
		{
			mDescription = description;
		}


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::logic::SequenceTrack methods   ]
		//[-------------------------------------------------------]
		inline void SequenceTrack::notifyTimePassed(float oldTime, float newTime) const
		{
			// Nothing to do by default
		}


		//[-------------------------------------------------------]
		//[ Protected methods                                     ]
		//[-------------------------------------------------------]
		inline SequenceTrack::SequenceTrack(Sequence* sequence, uint32 id) :
			mSequence(sequence),
			mId(id),
			mActive(true)
		{
			// Nothing to do in here
		}

		template <typename T>
		bool SequenceTrack::isInsideKeyInterval(const std::vector<T*>& keys, float oldFrame, float newFrame) const
		{
			if (keys.empty())
			{
				return false;
			}
			else
			{
				const uint32 roundedOldFrame = static_cast<uint32>(std::floor(oldFrame));
				const uint32 roundedNewFrame = static_cast<uint32>(std::ceil(newFrame));

				return (roundedNewFrame >= keys.front()->getFrameNumber() && roundedOldFrame <= keys.back()->getFrameNumber());
			}
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
