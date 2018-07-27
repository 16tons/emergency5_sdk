// Copyright (C) 2012-2017 Promotion Software GmbH


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
		inline Sequence::Sequence(uint64 id, SequenceManager& sequenceManager) :
			mId(id),
			mSequenceManager(&sequenceManager),
			mBeginTime(0.0f),
			mDuration(10.0f),
			mShouldLoop(false),
			mSuggestedPlaybackSpeed(1.0f),
			mFramesPerSecond(30)
		{
			// Nothing to do in here
		}

		inline Sequence::~Sequence()
		{
			deleteAllTracks();
		}

		inline uint64 Sequence::getId() const
		{
			return mId;
		}

		inline SequenceManager& Sequence::getSequenceManager() const
		{
			return *mSequenceManager;
		}

		inline float Sequence::getBeginTime() const
		{
			return mBeginTime;
		}

		inline void Sequence::setBeginTime(float newBeginTime)
		{
			mBeginTime = newBeginTime;
		}

		inline float Sequence::getDuration() const
		{
			return mDuration;
		}

		inline void Sequence::setDuration(float newDuration)
		{
			QSF_ASSERT(newDuration >= 0.0f, "Sequence duration has to be positive", QSF_REACT_THROW);

			mDuration = newDuration;
		}

		inline const Sequence::TrackList& Sequence::getTrackList() const
		{
			return mSequenceTracks;
		}

		inline SequenceTrackEntityComponentProperty* Sequence::addTrackForEntityComponentProperty(uint64 entityId, const std::string& componentPropertyPath)
		{
			return addTrackForEntityComponentPropertyWithId(entityId, componentPropertyPath, getFreeTrackId());
		}

		inline SequenceTrackEntityComponentProperty* Sequence::addTrackForEntityComponentProperty(const camp::Property& propertyForTrack)
		{
			return addTrackForEntityComponentPropertyWithId(propertyForTrack, getFreeTrackId());
		}

		inline SequenceTrackEntityComponentProperty* Sequence::addTrackForEntityComponentPropertyByTrackType(const camp::Class& trackClassType)
		{
			return addTrackForEntityComponentPropertyByTrackType(trackClassType, getFreeTrackId());
		}

		inline SequenceTrack* Sequence::addTrackByTrackType(const camp::Class& trackClassType)
		{
			return addTrackByTrackType(trackClassType, getFreeTrackId());
		}

		inline bool Sequence::getShouldLoop() const
		{
			return mShouldLoop;
		}

		inline void Sequence::setShouldLoop(bool shouldLoop)
		{
			mShouldLoop = shouldLoop;
		}

		inline float Sequence::getSuggestedPlaybackSpeed() const
		{
			return mSuggestedPlaybackSpeed;
		}

		inline void Sequence::setSuggestedPlaybackSpeed(float suggestedPlaybackSpeed)
		{
			mSuggestedPlaybackSpeed = suggestedPlaybackSpeed;
		}

		inline uint32 Sequence::getFramesPerSecond() const
		{
			return mFramesPerSecond;
		}

		inline void Sequence::setFramesPerSecond(uint32 newFramesPerSecond)
		{
			mFramesPerSecond = newFramesPerSecond;
		}


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		inline void Sequence::addTrack(SequenceTrack& sequenceTrack)
		{
			mSequenceTracks.push_back(&sequenceTrack);
		}

		inline float Sequence::getLocalTime(float time) const
		{
			const float rangeCorrectedTime = std::min(mBeginTime + mDuration, std::max(mBeginTime, time + mBeginTime));

			// Let's put it into "local time", i.e. partial frame numbers
			const float localTime = rangeCorrectedTime * mFramesPerSecond;

			return localTime;
		}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf
