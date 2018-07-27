// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/gui/GuiHotReloadSupport.h"


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
	*    EMERGENCY 5 freeplay end window HUD element
	*/
	class IngameFreeplayEndWindow : public GuiHotReloadSupport
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit IngameFreeplayEndWindow(qsf::GuiContext& guiContext);
		virtual ~IngameFreeplayEndWindow();

		void setFreeplayResults(uint32 score, const std::string& time);

		/**
		*  @brief
		*    Use this function to show the freeplay end window
		*
		*  @param[in] stopApplication
		*    If stopApplication true, the appliction will shutdown after the next button is clicked
		*/
		void showIngameFreeplayEndWindow(bool stopApplication = false);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Protected virtual em5::GuiHotReloadSupport methods    ]
	//[-------------------------------------------------------]
	protected:
		virtual void onReloadGui() override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		/**
		*  @brief
		*    Upload scores function for the results
		*/
		void publish(bool state);
		void setupGui();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	// TODO(co) Please, make this private
	public:
		uint32	mScore;				///< Hold the score
		uint32	mFailedEvents;		///< Hold all failed events
		uint32	mSuccessEvents;		///< Hold all success events
		bool	mStopApplication;	///< If this member true the appliction will stop if the player click the next button


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
