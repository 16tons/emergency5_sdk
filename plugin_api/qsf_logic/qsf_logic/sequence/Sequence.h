// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"

#include <qsf/reflection/object/Object.h>
#include <qsf/reflection/type/CampUint64.h>	// The 64 bit unsigned integer ID of the sequencer entity component property track class is connected to the CAMP reflection system, so we need this header to tell CAMP about this data type

#include <vector>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{
		class SequenceTrack;
		class SequenceManager;
		class SequenceTrackEntityComponentProperty;
	}
}
namespace camp
{
	class Class;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Sequence class
		*
		*  @note
		*    - Use "qsf::logic::SequencePlayer" for sequence playback
		*/
		class QSF_LOGIC_API_EXPORT Sequence : public Object
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
			friend class SequenceManager;	// Sequence manager must be able to change private attributes (like ID)


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			typedef std::vector<SequenceTrack*> TrackList;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*
			*  @param[in] id
			*    Unique sequence identifier
			*  @param[in] sequenceManager
			*    Sequence manager
			*/
			inline Sequence(uint64 id, SequenceManager& sequenceManager);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~Sequence();

			/**
			*  @brief
			*    Return the unique sequence identifier
			*
			*  @return
			*    The unique sequence identifier
			*
			*  @note
			*    - Connected to the CAMP reflection system
			*/
			inline uint64 getId() const;

			/**
			*  @brief
			*    Return the sequence manager this sequence is in
			*
			*  @return
			*    The sequence manager this sequence is in, do no destroy the returned instance
			*/
			inline SequenceManager& getSequenceManager() const;

			/**
			*  @brief
			*    Load a sequence by using a given filename
			*
			*  @param[in] filename
			*    UTF-8 filename in platform-independent notation of the sequence to load
			*
			*  @return
			*    "true" if all went fine, else "false" (file not found?)
			*
			*  @note
			*    - The data of this sequence instance is lost when loading a new one
			*
			*  @todo
			*    - TODO(oi) For now, we only support JSON
			*/
			bool loadByFilename(const std::string& filename);

			//[-------------------------------------------------------]
			//[ Playback                                              ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Evaluate the sequence at the given point in time
			*
			*  @param[in] time
			*    Point in time where to evaluate (given as seconds) in the range [0, duration]
			*
			*  @note
			*    - This might set component properties of the entities stored in the sequence
			*    - The same interface is provided by "notifyTimePassed()". So don't call "notifyTimePassed()" and evaluate in the same frame, it will duplicate work.
			*/
			void evaluate(float time) const;

			/**
			*  @brief
			*    Evaluate a particular track of the sequence at the given point in time
			*
			*  @param[in] trackId
			*    Track reference which should be evaluated
			*  @param[in] time
			*    Point in time where to evaluate (given as seconds) in the range [0, duration]
			*
			*  @note
			*    - This might set component properties of the entities stored in the sequence
			*    - The same interface is provided by "notifyTimePassed()". So don't call "notifyTimePassed()" and evaluate in the same frame, it will duplicate work.
			*/
			void evaluateTrack(uint32 trackId, float time) const;

			/**
			*  @brief
			*    Notify the sequence of passed time
			*
			*  @param[in] oldTime
			*    Point in time where the sequence was previously evaluated (given as seconds) in the range [0, duration]
			*  @param[in] newTime
			*    Current point in time where the sequence should be evaluated (given as seconds) in the range [0, duration]
			*
			*  @remarks
			*    This is the preferred interface for run-time as some tracks only work when they know the exact time which has
			*    passed and a simple evaluate-at-this-point-in-time would not suffice (e.g. game logic signaling). This might
			*    set component properties of the entities stored in the sequence.
			*/
			void notifyTimePassed(float oldTime, float newTime) const;

			/**
			*  @brief
			*    Return reference "time zero" in seconds
			*/
			inline float getBeginTime() const;

			/**
			*  @brief
			*    Change the reference "time zero" point in time (in seconds) to allow key times in the whole range of float
			*/
			inline void setBeginTime(float newBeginTime);

			/**
			*  @brief
			*    Return the duration of the sequence in seconds
			*/
			inline float getDuration() const;

			/**
			*  @brief
			*    Change the duration of the sequence in seconds
			*/
			inline void setDuration(float newDuration);

			/**
			*  @brief
			*    The "should loop" flag is informative data which is provided by the creator of a sequence to hint the runtime player if the sequence should be played in a loop
			*
			*  @todo
			*    - TODO(oi) Maybe this would be more appropriately treated as metadata of a "sequence playback" component which can be controlled by the designer in maps etc.
			*/
			inline bool getShouldLoop() const;
			inline void setShouldLoop(bool shouldLoop);

			/**
			*  @brief
			*    The "suggested playback speed" is informative data provided by the creator of the sequence to hint the runtime player of the best playback speed (is a factor, like 0.5x or 2x)
			*/
			inline float getSuggestedPlaybackSpeed() const;
			inline void setSuggestedPlaybackSpeed(float suggestedPlaybackSpeed);

			inline uint32 getFramesPerSecond() const;
			inline void setFramesPerSecond(uint32 newFramesPerSecond);

			//[-------------------------------------------------------]
			//[ Tracks                                                ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Return a non-mutable list of all tracks
			*/
			inline const TrackList& getTrackList() const;

			/**
			*  @brief
			*    Look up track instance by ID
			*
			*  @param[in] trackId
			*    Track reference which should be returned
			*
			*  @return
			*    Will return a null pointer if track is not found
			*/
			SequenceTrack* findTrack(uint32 trackId) const;

			/**
			*  @brief
			*    Remove a track from the sequences and deletes its memory
			*
			*  @param[in] trackId
			*    Track reference which should be deleted
			*
			*  @note
			*    - All references to this track are invalid after this call
			*/
			void removeAndDeleteTrack(uint32 trackId);

			/**
			*  @brief
			*    Delete all track instances and clear the track list
			*
			*  @note
			*    - All previously retrieved track instances become invalid after this call
			*/
			void deleteAllTracks();

			/**
			*  @brief
			*    Generate a track ID which is not yet taken in this sequence
			*/
			uint32 getFreeTrackId();

			/**
			*  @brief
			*    Generate multiple track IDs which are not present yet in this sequence (use it to preallocate unique track IDs if you want to know how to add multiple tracks with forced IDs)
			*/
			void getFreeTrackIds(std::vector<uint32>& freeTrackIdTargetList, uint32 count);

			//[-------------------------------------------------------]
			//[ Concrete track types                                  ]
			//[-------------------------------------------------------]
			/**
			*  @brief
			*    Add a specific track for a value type, e.g. "addTrackForEntityComponentProperty(camp::classByType<glm::vec3>())"
			*/
			inline SequenceTrackEntityComponentProperty* addTrackForEntityComponentProperty(uint64 entityId, const std::string& componentPropertyPath);
			SequenceTrackEntityComponentProperty* addTrackForEntityComponentPropertyWithId(uint64 entityId, const std::string& componentPropertyPath, uint32 newTrackId);

			inline SequenceTrackEntityComponentProperty* addTrackForEntityComponentProperty(const camp::Property& propertyForTrack);
			SequenceTrackEntityComponentProperty* addTrackForEntityComponentPropertyWithId(const camp::Property& propertyForTrack, uint32 newTrackId);

			SequenceTrackEntityComponentProperty* addTrackForEntityComponentPropertyByTrackType(const camp::Class& trackCampClassType, uint32 newTrackId);
			inline SequenceTrackEntityComponentProperty* addTrackForEntityComponentPropertyByTrackType(const camp::Class& trackCampClassType);

			SequenceTrack* addTrackByTrackType(const camp::Class& trackCampClassType, uint32 newTrackId);
			inline SequenceTrack* addTrackByTrackType(const camp::Class& trackCampClassType);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			/**
			*  @brief
			*    Add a new track to the sequence
			*
			*  @param[in] sequenceTrack
			*    New track to add to the sequence. Memory ownership is transferred to the sequence.
			*/
			inline void addTrack(SequenceTrack& sequenceTrack);

			const camp::Class* getTrackClassForPropertyClass(const camp::Class& campClassOfProperty) const;

			/**
			*  @brief
			*    Computes "local time", i.e. partial frame numbers, from a zero-based timeline
			*/
			inline float getLocalTime(float time) const;


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint64				mId;						///< The unique sequence identifier inside the associated manager
			SequenceManager*	mSequenceManager;			///< The manager this sequence is registered in
			float				mBeginTime;					///< The beginning time of the sequence (has not to be 0!) in seconds
			float				mDuration;					///< Duration of the sequence (has to be greater than 0) in seconds
			// TODO(oi): Maybe a map instead of a list would be better for per-ID lookup. Not runtime relevant, but edit time
			TrackList			mSequenceTracks;			///< List of track instances, managed by the sequence
			bool				mShouldLoop;				///< Informative data: Should the sequence be looped by the sequence player?
			float				mSuggestedPlaybackSpeed;	///< Informative data: Playback speed (as a factor, e.g. 0.5x or 2x) suggested for the sequence playback
			uint32				mFramesPerSecond;			///< Default playback rate in frames/second, defaults to 60


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/Sequence-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::logic::Sequence)
