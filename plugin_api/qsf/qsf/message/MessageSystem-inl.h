// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline MessageSystem::MessageSystem() :
		mRootManager(nullptr)
	{
		// Nothing to do in here
	}

	inline MessageSystem::~MessageSystem()
	{
		// Nothing to do in here
	}

	template<typename T>
	bool MessageSystem::emitMessageWithResponse(const MessageConfiguration& message, T& response)
	{
		MessageParameters parameters;
		emitMessage(message, parameters);
		return parameters.tryGetResponse(response);
	}

	template<typename T>
	bool MessageSystem::emitMessageWithResponse(const MessageConfiguration& message, T& response, MessageParameters& parameters)
	{
		emitMessage(message, parameters);
		return parameters.tryGetResponse(response);
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* MessageSystem::getName() const
	{
		return "Message system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
