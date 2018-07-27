// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline HintMessage::HintMessage() :
		mMessageType(MESSAGETYPE_HINT),
		mPlayerIndex(0)
	{
		// Nothing here
	}

	inline HintMessage::HintMessage(MessageType messageType, const std::string& messageText, const std::string& audioFile, uint32 playerIndex) :
		mMessageType(messageType),
		mMessageText(messageText),
		mAudioFile(audioFile),
		mPlayerIndex(playerIndex)
	{
		// Nothing here
	}

	inline HintMessage::~HintMessage()
	{
		// Nothing here
	}

	inline HintMessage::MessageType HintMessage::getMessageType() const
	{
		return mMessageType;
	}

	inline const std::string& HintMessage::getMessageText() const
	{
		return mMessageText;
	}

	inline const std::string& HintMessage::getAudioFile() const
	{
		return mAudioFile;
	}

	inline uint32 HintMessage::getPlayerIndex() const
	{
		return mPlayerIndex;
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
