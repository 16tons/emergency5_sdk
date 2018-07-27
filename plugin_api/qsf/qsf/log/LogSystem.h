// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf/Qsf.h"
#include "qsf/base/System.h"
#include "qsf/reflection/CampClass.h"

#include <boost/signals2/signal.hpp>
#include <boost/container/flat_map.hpp>


//[-------------------------------------------------------]
//[ Structures                                            ]
//[-------------------------------------------------------]
namespace qsf
{


	/**
	*  @brief
	*    Log message
	*
	*  @note
	*    - There's no "fatal" severity level by intent, use "QSF_ASSERT()" instead
	*/
	struct LogMessage
	{


		/**
		*  @brief
		*    Severity levels
		*
		*  @note
		*    - A GUI can use these severity levels to filter log messages
		*/
		enum SeverityLevel
		{
			TRACE,		///< Trace severity level
			DEBUG,		///< Debug severity level
			INFO,		///< Information severity level
			WARNING,	///< Warning severity level
			ERROR,		///< Error severity level
			NONE		///< None severity level, used to filter out everything
		};

		SeverityLevel severityLevel;	///< Severity level of the log message
		std::string   message;			///< Formatted UTF-8 log message
		uint32		  contextId;		///< Context ID

		bool operator ==(const LogMessage& other) const
		{
			return (severityLevel == other.severityLevel && message == other.message && contextId == other.contextId);
		}

		bool operator <(const LogMessage& other) const
		{
			return (severityLevel < other.severityLevel && message < other.message && contextId < other.contextId);
		}


	};


}	// qsf


//[-------------------------------------------------------]
//[ Macros                                                ]
//[-------------------------------------------------------]
/**
*  @brief
*    Log macro for printing text
*
*  @param[in] severityLevel
*    Severity level (TRACE, DEBUG, INFO, WARNING or ERROR)
*  @param[in] text
*    Text which should be written into the log
*/
#define QSF_LOG_PRINT(severityLevel, text) \
	{ \
		if (qsf::Qsf::instance() && qsf::Qsf::instance()->getLogSystem().shouldBeLogged(qsf::LogMessage::severityLevel)) \
		{ \
			const qsf::LogMessage::SeverityLevel severityLevelReal = qsf::LogMessage::SeverityLevel::severityLevel; /* Avoid 'expression is constant' warning */ \
			qsf::Qsf::instance()->getLogSystem().print(severityLevelReal, text); \
		} \
	}

/**
*  @brief
*    Log macro for printing text formatted using printf syntax
*
*  @param[in] severityLevel
*    Severity level (TRACE, DEBUG, INFO, WARNING or ERROR)
*  @param[in] text
*    Text which should be written into the log
*/
#define QSF_LOG_PRINTF(severityLevel, text, ...) \
	{ \
		if (qsf::Qsf::instance() && qsf::Qsf::instance()->getLogSystem().shouldBeLogged(qsf::LogMessage::severityLevel)) \
		{ \
			const qsf::LogMessage::SeverityLevel severityLevelReal = qsf::LogMessage::SeverityLevel::severityLevel; /* Avoid 'expression is constant' warning */ \
			qsf::Qsf::instance()->getLogSystem().printf(severityLevelReal, text, ##__VA_ARGS__); \
		} \
	}

/**
*  @brief
*    Log macro for printing text formatted using printf syntax and having an va_list as type
*
*  @param[in] severityLevel
*    Severity level (TRACE, DEBUG, INFO, WARNING or ERROR)
*  @param[in] text
*    Text which should be written into the log
*  @param[in] valist
*    Variable arguments already combined into a va_list type
*/
#define QSF_LOG_VAPRINTF(severityLevel, text, valist) \
	{ \
		if (qsf::Qsf::instance() && qsf::Qsf::instance()->getLogSystem().shouldBeLogged(qsf::LogMessage::severityLevel)) \
				{ \
			const qsf::LogMessage::SeverityLevel severityLevelReal = qsf::LogMessage::SeverityLevel::severityLevel; /* Avoid 'expression is constant' warning */ \
			qsf::Qsf::instance()->getLogSystem().vaprintf(severityLevelReal, text, valist); \
				} \
	}

/**
*  @brief
*    Log macro for printing text formatted as parameter to a stream
*
*  @param[in] severityLevel
*    Severity level (TRACE, DEBUG, INFO, WARNING or ERROR)
*  @param[in] text
*    Text which should be written into the log
*/
#define QSF_LOG_PRINTS(severityLevel, text) \
	{ \
		if (qsf::Qsf::instance() && qsf::Qsf::instance()->getLogSystem().shouldBeLogged(qsf::LogMessage::severityLevel)) \
		{ \
			std::ostringstream internalLogStream; \
			internalLogStream << text; \
			const qsf::LogMessage::SeverityLevel severityLevelReal = qsf::LogMessage::SeverityLevel::severityLevel; /* Avoid 'expression is constant' warning */ \
			qsf::Qsf::instance()->getLogSystem().print(severityLevelReal, internalLogStream.str().c_str()); \
		} \
	}

/**
*  @brief
*    Log macro for setting the current logging context to the ID of the active module
*
*  @param[in] contextId
*    An unique ID for the system. Create this from "qsf::StringHash" with the system name as parameter.
*
*  @note
*    - The current context will only be set if the previous context was zero
*    - Call this whenever some system that should use the severity override mechanism starts its update
*    - The current context is a thread specific variable so it is safe to call this in a multithreaded environment
*/
#define QSF_LOG_ENTER_CONTEXT(contextId) \
	{ \
		if (qsf::Qsf::instance() && 0 == qsf::Qsf::instance()->getLogSystem().getCurrentContext()) \
		{ \
			qsf::Qsf::instance()->getLogSystem().setCurrentContext(contextId); \
		} \
	}

/**
*  @brief
*    Log macro for clearing the current logging context indicator and replace it with zero, the indicator that no special context is currently active
*
*  @param[in] contextId
*    An unique ID for the system. Create this from "qsf::StringHash" with the system name as parameter.
*
*  @note
*    - The current context will only be cleared if the previous context was the one specified
*    - Call this whenever the update of a system is left that has set itself as special context at the beginning of its update
*    - The current context is a thread specific variable so it is safe to call this in a multithreaded environment
*/
#define QSF_LOG_LEAVE_CONTEXT(contextId) \
	{ \
		if (qsf::Qsf::instance() && (contextId) == qsf::Qsf::instance()->getLogSystem().getCurrentContext()) \
		{ \
			qsf::Qsf::instance()->getLogSystem().clearCurrentContext(); \
		} \
	}


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Log system class
	*
	*  @remarks
	*    The most basic usage looks like the following:
	*    @code
	*    QSF_LOG_PRINT(INFO, "System started");
	*    QSF_LOG_PRINTF(ERROR, "The number %d is only half the truth", 21);
	*    @endcode
	*
	*    The log will be written into the user data directory by using the organization name and application name
	*    stored within the application context used by the QSF instance. Examples on different systems:
	*    - Windows 7:  "C:/Users/<UserName>/AppData/Roaming/Promotion Software GmbH/QSF Sample"
	*    - Windows XP: "C:/Documents and Settings/<Username>/Application Data/Promotion Software GmbH/QSF Sample"
	*    - Linux:      "/home/<username>/Promotion Software GmbH/QSF Sample"
	*
	*    The logging system has a default minimum severity and allows individual contexts to provide overrides.
	*    This allows debugging single systems by selectively tracing their debug output.
	*    Each system that wants to use this service needs to register the begin and end of its main activation phase with the logging system.
	*    This is done with a unique ID as index which should be created consistently as a string hash from the systems name.
	*    Zero is used as the default here and is an indication that no specific context is currently active.
	*
	*  @note
	*    - The external library "Boost.Log" ( http://sourceforge.net/projects/boost-log/ ) is used
	*/
	class QSF_API_EXPORT LogSystem : public System
	{


	//[-------------------------------------------------------]
	//[ Public Boost signals                                  ]
	//[-------------------------------------------------------]
	public:
		boost::signals2::signal<void (const LogMessage&)> NewMessage;	///< This Boost signal is emitted as soon as a new log message arrives, log message as first parameter


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor allowing to set the initial minimum severity for messages to be logged
		*
		* @param[in] logMinimum
		*	The initial minimum severity for messages to be logged
		*/
		inline explicit LogSystem(LogMessage::SeverityLevel logMinimum = LogMessage::INFO);

		/**
		*  @brief
		*    Destructor
		*/
		inline virtual ~LogSystem();

		/**
		*  @brief
		*    Get current number of buffered log messages
		*
		*  @return
		*    Number of buffered log messages
		*/
		inline uint32 getBufferCount() const;

		/**
		*  @brief
		*    Set how many log messages should be buffered in memory
		*
		*  @param[in] bufferCount
		*    Number of messages to buffer
		*/
		void setBufferCount(uint32 bufferCount);

		/**
		*  @brief
		*    Get a copy of the list which holds the last log messages
		*
		*  @return
		*    List of n-last UTF-8 log messages, do not destroy the list instance
		*/
		inline std::list<LogMessage>& getLastMessages();

		/**
		*  @brief
		*    Return the log file output directory
		*
		*  @return
		*    UTF-8 log file output directory, do not destroy the list instance (example: "C:/Users/<UserName>/AppData/Roaming/Promotion Software GmbH/EMERGENCY 5")
		*/
		inline const std::string& getLogFileOutputDirectory() const;

		/**
		*  @brief
		*    Return the latest log file path
		*
		*  @return
		*    UTF-8 log file path (example: "C:/Users/<UserName>/AppData/Roaming/Promotion Software GmbH/EMERGENCY 5/log/qsf_20141207_144118_00001.log")
		*/
		std::string getLatestLogFilePath() const;

		/**
		*  @brief
		*    Print a ASCII text into the log
		*
		*  @param[in] severityLevel
		*    Severity level
		*  @param[in] text
		*    Null terminated ASCII text which should be written into the log, in case of a null pointer nothing happens
		*/
		void print(LogMessage::SeverityLevel severityLevel, const char* text);

		/**
		*  @brief
		*    Print a formatted ASCII text into the log
		*
		*  @param[in] severityLevel
		*    Severity level
		*  @param[in] text
		*    Null terminated ASCII text which should be written into the log, in case of a null pointer nothing happens
		*/
		void printf(LogMessage::SeverityLevel severityLevel, const char* text, ...);

		/**
		*  @brief
		*    Print a formatted ASCII text into the log
		*
		*  @param[in] severityLevel
		*    Severity level
		*  @param[in] text
		*    Null terminated ASCII text which should be written into the log, in case of a null pointer nothing happens
		*  @param[in] va_list
		*    The variable arguments already combined into a va_list
		*/
		void vaprintf(LogMessage::SeverityLevel severityLevel, const char* text, va_list vaList);

		/**
		*  @brief
		*    Print a Unicode text into the log
		*
		*  @param[in] severityLevel
		*    Severity level
		*  @param[in] text
		*    Null terminated Unicode text which should be written into the log, in case of a null pointer nothing happens
		*/
		void print(LogMessage::SeverityLevel severityLevel, const wchar_t* text);

		/**
		*  @brief
		*    Print a formatted Unicode text into the log
		*
		*  @param[in] severityLevel
		*    Severity level
		*  @param[in] text
		*    Null terminated Unicode text which should be written into the log, in case of a null pointer nothing happens
		*/
		void printf(LogMessage::SeverityLevel severityLevel, const wchar_t* text, ...);

		/**
		*  @brief
		*    Print a formatted Unicode text into the log
		*
		*  @param[in] severityLevel
		*    Severity level
		*  @param[in] text
		*    Null terminated Unicode text which should be written into the log, in case of a null pointer nothing happens
		*  @param[in] va_list
		*    The variable arguments already combined into a va_list
		*/
		void vaprintf(LogMessage::SeverityLevel severityLevel, const wchar_t* text, va_list vaList);

		/**
		*  @brief
		*    Determine whether a message with the given severity should be logged from the current context
		*
		*  @param[in] severity
		*    The severity level of the upcoming, potential log entry
		*
		*  @return
		*    True if a message of the given severity should be logged, otherwise false
		*
		*  @note
		*    - This allows to avoid the unecessary creation on the stream
		*/
		bool shouldBeLogged(LogMessage::SeverityLevel severity) const;

		/**
		*  @brief
		*    Get the current logging context
		*
		*  @note
		*    - This is needed to add the current context to new messages
		*/
		uint32 getCurrentContext() const;

		/**
		*  @brief
		*    Set the current logging context to the ID of the active module
		*
		*  @param[in] contextId
		*    An unique ID for the system. Create this from "qsf::StringHash" with the system name as parameter.
		*
		*  @note
		*    - Call this whenever some system that should use the severity override mechanism starts its update
		*    - The current context is a thread specific variable so it is safe to call this in a multithreaded environment
		*/
		void setCurrentContext(uint32 contextId) const;

		/**
		*  @brief
		*    Clear the current logging context indicator and replace it with zero, the indicator that no special context is currently active
		*
		*  @note
		*    - Call this whenever the update of a system is left that has set itself as special context at the beginning of its update
		*    - The current context is a thread specific variable so it is safe to call this in a multithreaded environment
		*/
		void clearCurrentContext() const;

		/**
		*  @brief
		*    Set the default minimum severity level that should be logged
		*
		*  @param[in] logMinimum
		*    The new default minimum severity level that is logged
		*
		*  @remarks
		*    This is used as a filter if for the current special context no override is set or we are currently not inside any specific context.
		*/
		inline void setDefaultMinimumSeverityToLog(LogMessage::SeverityLevel logMinimum);

		/**
		*  @brief
		*    Override the global minimum severity to log for a specific context
		*
		*  @param[in] logMinimum
		*    The new minimum severity that should be logged for the specified context
		*  @param[in] contextId
		*    The unique ID of the context for which a new minimum severity to log should be used.
		*    Create the ID consistently from "qsf::StringHash" with the system name as parameter.
		*/
		inline void setSeverityOverrideForContext(LogMessage::SeverityLevel logMinimum, uint32 contextId);

		/**
		*  @brief
		*    Reset the minimum severity to log for a specific context to use the default from now on
		*
		*  @param[in] contextId
		*    The unique ID of the context that should use the default minimum severity to log.
		*    Create the ID consistently from "qsf::StringHash" with the system name as parameter.
		*
		*  @note
		*    - Discards any override that might have been set for this context earlier on
		*/
		inline void clearSeverityOverrideForContext(uint32 contextId);

		/**
		*  @brief
		*    Register another logging context
		*
		*  @param[in] contextId
		*    The ID that is used during logging
		*  @param[in] readableName
		*    The plain text namke for the system. This is shown as the option in the log config screen.
		*    It is good practice to use the string that the ID was hashed from.
		*
		*  @note
		*    - Every logging context needs to register itself here before it can be used for context specific logging
		*/
		inline void addContext(uint32 contextId, const std::string& readableName);

		/**
		*  @brief
		*    Unregister a logging context
		*
		*  @param[in] contextId
		*    The ID that was used during logging
		*
		*  @remarks
		*    While not strictly necessary you can clean up the logging context registry this way.
		*    Only call when it is absolutely clear that no logging calls with this context can be generated afterwards.
		*    Does nothing if the context was not registered for logging.
		*/
		inline void removeContext(uint32 contextId);

		/**
		*  @brief
		*    Return all registered logging contexts
		*
		*  @remarks
		*    This returns the contexts as a map from context ID to plain text string to show.
		*/
		inline const boost::container::flat_map<uint32, std::string>& getContexts() const;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::System methods                    ]
	//[-------------------------------------------------------]
	public:
		inline virtual const char* getName() const override;
		//[-------------------------------------------------------]
		//[ Lifecycle                                             ]
		//[-------------------------------------------------------]
		virtual bool onStartup(bool serverMode) override;
		virtual void onShutdown() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    The function registers debugger output sink in the Boost logging library
		*/
		void registerBoostLogDebugOutput();

		/**
		*  @brief
		*    The function registers file output sink in the Boost logging library
		*/
		void registerBoostLogFileOutput(bool serverMode);

		/**
		*  @brief
		*    The function registers file record sink in the Boost logging library
		*/
		void registerBoostLogRecordOutput();

		void registerBoostLogConsoleOutput();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		typedef boost::container::flat_map<uint32, LogMessage::SeverityLevel> SeverityOverrides;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32						mBufferCount;				///< Number of messages to buffer
		std::list<LogMessage>		mLastMessages;				///< List buffering formatted UTF-8 log messages
		std::string					mLogFileOutputDirectory;	///< UTF-8 log file output directory (example: "C:/Users/<UserName>/AppData/Roaming/Promotion Software GmbH/EMERGENCY 5")

		LogMessage::SeverityLevel	mDefaultMinimalSeverityToLog;	///< If no specific override for the current context is defined, this is the minimum severity for messages to get logged

		SeverityOverrides								mMinimumSeverityLevelOverrides;	///< Table of minimum logging severity overrides for specific contexts
		boost::container::flat_map<uint32, std::string> mRegisteredContexts;			///< Every known context is listed here, first parameter is the hashId, second the humanreadble name


	};

	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	/**
	*  @brief
	*    Scoped log context ease-of-use class
	*/
	class ScopedLogContext
	{
	public:
		/**
		*  @brief
		*    Set the current logging context to the ID of the active module
		*
		*  @param[in] contextId
		*    An unique ID for the system. Create this from "qsf::StringHash" with the system name as parameter.
		*
		*  @note
		*    - The current context will only be set if the previous context was zero
		*    - Call this whenever some system that should use the severity override mechanism starts its update
		*    - The current context is a thread specific variable so it is safe to call this in a multithreaded environment
		*/
		inline ScopedLogContext(uint32 contextId) :
			mContextId(contextId)
		{
			QSF_LOG_ENTER_CONTEXT(mContextId);
		}

		/**
		*  @brief
		*    Clear the current logging context indicator and replace it with zero, the indicator that no special context is currently active
		*
		*  @note
		*    - The current context will only be cleared if the previous context was the one specified
		*    - The current context is a thread specific variable so it is safe to call this in a multithreaded environment
		*/
		inline ~ScopedLogContext()
		{
			QSF_LOG_LEAVE_CONTEXT(mContextId);
		}

		const uint32 mContextId;
	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf/log/LogSystem-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::LogMessage::SeverityLevel)
