// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/job/JobProxy.h>
#include <qsf/math/Color3.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace em5
{


	//[-------------------------------------------------------]
	//[ Classes                                               ]
	//[-------------------------------------------------------]
	class EM5_API_EXPORT MessageWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MessageWindow(qsf::GuiContext& guiContext);
		virtual ~MessageWindow();
		void setMaximumNumberOfMessages(uint32 maximumNumberOfMessages);

		/**
		*  @brief
		*    Add message text to the window
		*/
		void addTextMessage(const std::string& text, const qsf::Color3& color = qsf::Color3::WHITE);

		/**
		*  @brief
		*    Removes all text from the window
		*/
		void clear();


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


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Process the animation
		*/
		void onAnimationUpdate(const qsf::JobArguments& jobArguments);

		/**
		*  @brief
		*    Open the message window
		*/
		void open();

		/**
		*  @brief
		*    Close the message window
		*/
		void close();


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		struct NewMessageData
		{
			Rocket::Core::Element*	element;	///< Holds the close event objective window element
			float					height;		///< Holds height of the element
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		uint32						mMaximumNumberOfMessages;
		qsf::JobProxy				mAnimationJobProxy;				///< Regular animation update job proxy; for updates once a frame; insert new message animation
		std::vector<NewMessageData>	mNewMessageTextElementsList;	///< Hold a list of the new message which have to insert in the message window


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
