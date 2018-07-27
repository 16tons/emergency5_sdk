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


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace Rocket
{
	namespace Core
	{
		class Event;
	}
}
namespace em5
{
	class ProgressBar;
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
	*    Patch update pregress dialog GUI window
	*/
	class PatchUpdateProgressDialog : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum AnimationState
		{
			NONE,
			BUTTON_OUT,
			PROGRESS_IN
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit PatchUpdateProgressDialog(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~PatchUpdateProgressDialog();

		/**
		*  @brief
		*    Set the progressbar percentage
		*
		*  @param[in] percentage
		*    Percentage between 0.0 and 1.0
		*/
		void setPercentage(float percentage);

		/**
		*  @brief
		*    Set the complete progressbar progressbar percentage
		*
		*  @param[in] percentage
		*    Percentage between 0.0 and 1.0
		*/
		void setCompletePercentage(float percentage);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual bool onPreShow() override;
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onUpdateAnimation(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy	mAnimationJobProxy;	///< Regular animation update job proxy; for updates once a frame
		AnimationState	mState;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
