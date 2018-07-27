// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/game/hint/HintMessage.h"

#include <qsf/audio/AudioProxy.h>
#include <qsf/time/Time.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class JobArguments;
}
namespace em5
{
	class SupervisorWindow;
}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Hint system knows how to trigger hints and delegates hint calls from gameplay to GUI
	*/
	class EM5_API_EXPORT HintSystem : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Default constructor
		*/
		HintSystem();

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~HintSystem();

		/**
		*  @brief
		*    Enqueue a message in the hint system
		*/
		void addMessage(const std::string& messageId, HintMessage::MessageType messageType, uint32 playerIndex = qsf::getUninitialized<uint32>(0), qsf::Time blockedTime = qsf::Time::ZERO);

		/**
		*  @brief
		*    Update other windows and detects starting new hints
		*/
		void update(const qsf::JobArguments& jobArguments);

		/**
		*  @brief
		*    Show the last hint again
		*/
		void repeatLastHint();

		/**
		*  @brief
		*    Stop the current hint
		*/
		void stopCurrentHint();

		/**
		*  @brief
		*    Remove any not played hint from the list
		*/
		void clear();

		bool isSupervisorWindowOpen() const;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void playHint(const HintMessage& hintMessage, bool ignoreAudioBlock = false);
		uint64 getHintPlayerKey(const std::string& messageId, uint32 playerId);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		std::list<HintMessage>	mMessages;
		std::list<HintMessage>	mMessageHistory;
		SupervisorWindow&		mSupervisorWindow;
		qsf::Time				mCurrentTime;
		qsf::JobProxy			mUpdateJobProxy;

		std::map<uint64, qsf::Time>	mBlockedHints;		///< Hints blocked for a variable amount of time. Don't play or queue hints from this list. As key we use the playerId (upper 32bit) and the messageId (lower 32bit).
		std::map<uint64, qsf::Time>	mRecentAudioHints;	///< Hints recently played including audio; these should not trigger any audio output or supervisor until a certain time is gone


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
