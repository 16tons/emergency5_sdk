// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_logic/Export.h"

#include <qsf/job/JobProxy.h>
#include <qsf/base/GetUninitialized.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace logic
	{
		class Sequence;
		class PlaySequenceComponent;
	}
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
		*    Sequence player class
		*/
		class QSF_LOGIC_API_EXPORT SequencePlayer : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Default constructor
			*/
			SequencePlayer();

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~SequencePlayer();

			inline PlaySequenceComponent* getOwnerComponent() const;
			inline void setOwnerComponent(PlaySequenceComponent* component);

			inline bool isPlaying() const;

			inline float getPlaybackSpeed() const;
			inline void setPlaybackSpeed(float newSpeed);

			inline bool getIsLooping() const;
			inline void setIsLooping(bool isLooping);

			/**
			*  @brief
			*    Change the sequence this player is operating on
			*
			*  @remarks
			*    Changes the sequence this player is operating on, which can be a null pointer. In case of a valid sequence reference,
			*    the sequence will be evaluated immediately at the current playhead time. The playhead time will be adjusted beforehand
			*    to stay in the valid range. Might expose some funny behavior when looping is enabled and the playhead is not at the
			*    start of the sequence. The sequence informative data is evaluated and applied to the current player settings (e.g. looping and playback speed).
			*/
			inline Sequence* getSequence() const;
			void setSequence(Sequence* sequence, const StringHash& jobManagerId = getUninitialized<StringHash>());

			/**
			*  @brief
			*    Advance the playhead and evaluates the sequence
			*
			*  @param[in] deltaTime
			*    Time passed since last playhead advancement (frame time delta) in seconds
			*
			*  @return
			*    "true" in case the sequence is going to continue playback, "false" if it has ended and should be removed
			*
			*  @todo
			*    - TODO(co) We might want to use our "qsf::Time"-class in here
			*/
			bool advancePlayhead(float deltaTime);

			/**
			*  @brief
			*    Will only record the time as the internal current time state
			*
			*  @param[in] newTime
			*    Zero-based timestamp in seconds where the playhead is set to
			*
			*  @note
			*    - If you need to play randomly inside a sequence, use "modifyTime()" to change the playhead at once which won't evaluate the sequence
			*
			*  @todo
			*    - TODO(co) We might want to use our "qsf::Time"-class in here
			*/
			inline void modifyTime(float newTime);

			/**
			*  @brief
			*    Return the internal playhead time
			*/
			inline float getTime() const;


		//[-------------------------------------------------------]
		//[ Protected virtual qsf::logic::SequencePlayer methods  ]
		//[-------------------------------------------------------]
		protected:
			virtual void onStartSequence();
			virtual void onEndSequence();
			virtual void onNotifyTimePassed(float oldTime, float newTime);


		//[-------------------------------------------------------]
		//[ Private methods                                       ]
		//[-------------------------------------------------------]
		private:
			void updateJob(const JobArguments& jobArguments);
			void endSequence();
			void preloadReferencedResources();


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			float	  mCurrentTime;
			float	  mPlaybackSpeed;
			bool	  mIsLooping;
			JobProxy  mUpdateJobProxy;			///< Job proxy
			Sequence* mCurrentRunningSequence;	///< Currently played sequence; may be a null pointer, do not destroy the instance
			PlaySequenceComponent* mOwnerComponent;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // logic
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_logic/sequence/SequencePlayer-inl.h"
