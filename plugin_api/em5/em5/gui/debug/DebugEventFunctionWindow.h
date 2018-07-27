// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/base/NamedIdentifier.h>
#include <qsf/gui/GuiDocument.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class FreeplayEvent;
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
	*    In-game debug window for triggering specific actions for the running event
	*/
	class DebugEventFunctionWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit DebugEventFunctionWindow(qsf::GuiContext& guiContext);

		bool updateAndCheckButton(const qsf::NamedIdentifier& buttonName, FreeplayEvent& freeplayEvent);


	//[-------------------------------------------------------]
	//[ Public virtual methods                                ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		Rocket::Core::Element* addButton(const std::string& buttonName, const std::string& freeplayEventName, const std::string& rocketEventName);
		void updateJob(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		struct Button
		{
			qsf::NamedIdentifier mName;
			Rocket::Core::Element* mElement = nullptr;
			qsf::NamedIdentifier mRocketEventName;
			bool mWasPressed = false;
			qsf::Time mTimeSinceLastUpdate;
		};
		typedef std::map<uint32, Button> ButtonList;

		struct EventData
		{
			uint32 mFreeplayEventId;
			ButtonList mButtons;
			EventData(uint32 freeplayEventId) : mFreeplayEventId(freeplayEventId) {}
		};
		std::map<uint32, EventData> mEvents;

		qsf::JobProxy mUpdateJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
