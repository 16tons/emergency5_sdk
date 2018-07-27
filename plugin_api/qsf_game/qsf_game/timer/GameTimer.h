// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_game/Export.h"

#include <qsf/base/manager/ElementManager.h>
#include <qsf/message/MessageConfiguration.h>
#include <qsf/time/Time.h>

#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace game
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    Game timer implementation
		*/
		class QSF_GAME_API_EXPORT GameTimer : public boost::noncopyable
		{


		//[-------------------------------------------------------]
		//[ Friends                                               ]
		//[-------------------------------------------------------]
		friend class GameTimerManager;


		//[-------------------------------------------------------]
		//[ Public methods                                        ]
		//[-------------------------------------------------------]
		public:
			/**
			*  @brief
			*    Constructor
			*/
			inline GameTimer(uint32 id);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~GameTimer();

			/**
			*  @brief
			*    Return the timer's unique ID
			*/
			inline uint32 getId() const;

			/**
			*  @brief
			*    Return the message configuration
			*/
			inline const MessageConfiguration& getMessageConfiguration() const;

			/**
			*  @brief
			*    Return the remaining time until the timer fires
			*/
			inline const Time& getRemainingTime() const;

			/**
			*  @brief
			*    Reset an existing timer
			*/
			inline void resetTimer(const MessageConfiguration& messageConfiguration, const Time& remainingTime);

			/**
			*  @brief
			*    Trigger the timer immediately
			*/
			bool triggerNow();

			/**
			*  @brief
			*    Pauses the timer
			*/
			inline void pause();

			/**
			*  @brief
			*    Resumes the timer
			*/
			inline void resume();

			/**
			*  @brief
			*    Update this timers
			*/
			bool updateTimer(const Time& timePassed);

			/**
			*  @brief
			*    Serialization of the timer
			*/
			void serialize(BinarySerializer& serializer);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			uint32					mId;					///< Timer unique ID
			MessageConfiguration	mMessageConfiguration;	///< Configuration of message to emit when the timer fires
			Time					mRemainingTime;			///< Remaining time until the timer fires
			bool					mPaused;
			bool					mValid;


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // game
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_game/timer/GameTimer-inl.h"
