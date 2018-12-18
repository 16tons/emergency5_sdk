// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <boost/noncopyable.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class ToolTipWindow;
}
namespace qsf
{
	class GuiContext;
}
namespace Rocket
{
	namespace Core
	{
		class EventListener;
		class Event;
		class Element;
		class ElementDocument;
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
	*   EMERGENCY 5 tooltip manager class
	*/
	class ToolTipManager : public boost::noncopyable
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit ToolTipManager(qsf::GuiContext& guiContext);
		virtual ~ToolTipManager();
		qsf::GuiContext& getGuiContext() const;

		bool isActive() const;
		void setActive(bool active);
		void forceRefresh();	// Call this method in case you need an instance tooltip refresh in case of tooltip content changes

		// Configuration
		bool isFadeOutEnabled() const;
		void enabledFadeOut(bool enable);

		float getFadeOutTimeInSeconds() const;
		void setFadeOutTimeInSeconds(float fadeTime);

		bool isShowDelayEnabled() const;
		void enabledShowDelay(bool enable);

		float getShowDelayInSeconds() const;
		void setShowDelayInSeconds(float fadeTime);

		// This method is called by the event listener
		void handleRocketEvent(Rocket::Core::Event& event);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onReloadGui();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::GuiContext&			 mGuiContext;	///< Used GUI context, do not destroy the instance
		bool						 mIsActive;
		glm::ivec2					 mMousePosition;
		Rocket::Core::EventListener* mEventListener;
		Rocket::Core::Element*		 mCurrentTooltipElement;
		ToolTipWindow*				 mToolTipWindow;
		boost::signals2::connection  mReloadGuiConnection;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
