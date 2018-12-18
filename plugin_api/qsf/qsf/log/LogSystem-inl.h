// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	inline LogSystem::LogSystem(LogMessage::SeverityLevel logMinimum) :
		mBufferCount(1000),
		mDefaultMinimalSeverityToLog(logMinimum)
	{
		// Nothing to do in here
	}

	inline LogSystem::~LogSystem()
	{
		// Nothing to do in here
	}

	inline uint32 LogSystem::getBufferCount() const
	{
		return mBufferCount;
	}

	inline std::list<LogMessage>& LogSystem::getLastMessages()
	{
		return mLastMessages;
	}

	inline const std::string& LogSystem::getLogFileOutputDirectory() const
	{
		return mLogFileOutputDirectory;
	}

	inline void LogSystem::setDefaultMinimumSeverityToLog(LogMessage::SeverityLevel logMinimum)
	{
		mDefaultMinimalSeverityToLog = logMinimum;
	}

	inline void LogSystem::setSeverityOverrideForContext(LogMessage::SeverityLevel logMinimum, uint32 contextId)
	{
		mMinimumSeverityLevelOverrides[contextId] = logMinimum;
	}

	inline void LogSystem::clearSeverityOverrideForContext(uint32 contextId)
	{
		mMinimumSeverityLevelOverrides.erase(contextId);
	}

	inline void LogSystem::addContext(uint32 contextId, const std::string& readableName)
	{
		mRegisteredContexts.emplace(contextId, readableName);
	}

	inline void LogSystem::removeContext(uint32 contextId)
	{
		mRegisteredContexts.erase(contextId);
	}

	inline const boost::container::flat_map<uint32, std::string>& LogSystem::getContexts() const
	{
		return mRegisteredContexts;
	}


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	inline const char* LogSystem::getName() const
	{
		return "Log system";
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf
