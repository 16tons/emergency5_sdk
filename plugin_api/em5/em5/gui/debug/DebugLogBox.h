// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>
#include <qsf/job/JobProxy.h>

#include <mutex>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	struct LogMessage;
}
namespace Rocket
{
	namespace Core
	{
		class ElementText;
	}
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
	*    Debug GUI window
	*/
	class DebugLogBox : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit DebugLogBox(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~DebugLogBox();

		/**
		*  @brief
		*    Perform a GUI rebuild
		*/
		void rebuildGui();


		//[-------------------------------------------------------]
		//[ Public virtual qsf::GuiDocument methods               ]
		//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private virtual qsf::GuiDocument methods              ]
	//[-------------------------------------------------------]
	private:
		virtual bool onPreShow() override;
		virtual void onPreHide() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Called when a new log message has been added
		*
		*  @param[in] message
		*    The new UTF-8 log message
		*/
		void onNewLogMessage(const qsf::LogMessage& message);

		/**
		*  @brief
		*    Append a log message
		*
		*  @param[in] message
		*    The UTF-8 log message to append
		*/
		void appendLogMessage(const qsf::LogMessage& message);

		/**
		*  @brief
		*    Show the log messages in the GUI
		*/
		void showLogMessages();

		/**
		*  @brief
		*    Adds any new log messages to the ui
		*/
		void onUpdate(const qsf::JobArguments& arguments);

		void createOrUpdateLogMessagesAboveEntry();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool mAllowAutoOpen;

		std::mutex	mLogMessagesMutex;				//< Mutex to restrict access to mLogMessages
		std::vector<qsf::LogMessage> mLogMessages;	//< Holds a list of new log messages

		qsf::JobProxy	mUpdateJobProxy;

		Rocket::Core::Element* mMessageContainerElement;
		Rocket::Core::Element* mMessagesAboveElement;
		size_t mCurrentMessageAboveCount;
		size_t mMaxLogMessages;		//< How many log message should be shown


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
