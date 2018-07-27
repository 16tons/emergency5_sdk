// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <string>


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
	*    Hint message contains information about the text, sound file, video file the hint system needs
	*/
	class EM5_API_EXPORT HintMessage
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum MessageType
		{
			MESSAGETYPE_UNKNOWN,				///< Unknown message type
			MESSAGETYPE_HINT,					///< Hint with text message and audio output
			MESSAGETYPE_TEXTHINT,				///< Hint with text message, but no audio output
			MESSAGETYPE_UNITHINT,				///< Unit hint with audio output only
			MESSAGETYPE_SUPERVISOR_VIDEO,		///< Supervisor message with text and video + audio output, attention! We don't use videos anymore. Now we use RTT
			MESSAGETYPE_SUPERVISOR_AUDIO,		///< Supervisor message with text and audio output
			MESSAGETYPE_SUPERVISOR_PURE_AUDIO	///< Supervisor message with audio output only, no text
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		inline HintMessage();
		inline HintMessage(MessageType messageType, const std::string& messageText, const std::string& audioFile, uint32 playerIndex);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~HintMessage();

		inline MessageType getMessageType() const;
		inline const std::string& getMessageText() const;
		inline const std::string& getAudioFile() const;
		inline uint32 getPlayerIndex() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		MessageType mMessageType;
		std::string mMessageText;
		std::string mAudioFile;
		uint32		mPlayerIndex;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/game/hint/HintMessage-inl.h"
