// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"
#include "em5/gui/GuiHotReloadSupport.h"

#include <qsf/job/JobProxy.h>

#include <boost/function.hpp>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
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
	*    Message box GUI element class
	*/
	class EM5_API_EXPORT MessageBox : public GuiHotReloadSupport
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit MessageBox(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Setup the text and the callback function
		*/
		void setup(const std::string& title, const std::string& text, const std::string& buttonTrueText, const std::string& buttonFalseText, const boost::function<void(bool)>& callback);
		void setup(const std::string& title, const std::string& text, const std::string& buttonTrueText, const std::string& buttonFalseText, float autoFalseTime, const boost::function<void(bool)>& callback);
		void setup(const std::string& title, const std::string& text, const std::string& buttonText, const boost::function<void(bool)>& callback);
		void setup(const std::string& title, const std::string& text, const std::string& buttonText);

		void abort();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Process the button events
		*/
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;
		virtual void onPostShow() override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GuiHotReloadSupport methods    ]
	//[-------------------------------------------------------]
	protected:
		virtual void onReloadGui() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupUI();
		void setupTitleAndText(const std::string& title, const std::string& text);
		void setupOneButton(const std::string& buttonText);
		void setupTwoButtons(const std::string& buttonTrueText, const std::string& buttonFalseText);
		void showTime(bool show);

		void updateTime(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		boost::function<void(bool)> mCallbackFunction;	///< This function must be valid!
		float						mShowUpTime;		///< Saves the time when the message box calls show
		float						mAutoFalseTime;		///< After this time the message box will trigger the false button
		qsf::JobProxy				mJobProxy;			///< Regular job proxy; for updates once a second

		Rocket::Core::Element* mMessageBoxElement;

		Rocket::Core::Element*	   mButtonsContainerElement;
		Rocket::Core::Element*	   mTrueButtonElement;
		Rocket::Core::ElementText* mTrueButtonTextElement;
		Rocket::Core::Element*	   mFalseButtonElement;
		Rocket::Core::ElementText* mFalseButtonTextElement;

		Rocket::Core::Element*	   mTimeElement;
		Rocket::Core::ElementText* mTimeTextElement;
		Rocket::Core::ElementText* mCaptionTextElement;
		Rocket::Core::Element*	   mTextElement;
		Rocket::Core::ElementText* mTextTextElement;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
