// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/job/JobProxy.h>
#include <qsf/gui/GuiDocument.h>
#include <qsf/input/InputListener.h>
#include <qsf/time/Stopwatch.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class PipelineStateCompiler;
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
	*    EMERGENCY 5 material cache heating dialog window GUI
	*/
	class MaterialCacheHeatingDialog : public qsf::GuiDocument, public qsf::InputListener
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		enum AnimationState
		{
			NONE,
			PROGRESS_IN,
			PROGRESS_PHASE_1,
			PROGRESS_PHASE_2,
			PROGRESS_PHASE_3,
			PROGRESS_FINISHED,
			NUMBER_OF_STATES
		};


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit MaterialCacheHeatingDialog(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~MaterialCacheHeatingDialog();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Public virtual qsf::InputListener methods             ]
	//[-------------------------------------------------------]
	public:
		virtual void eventReceived(const qsf::InputEvent& inputEvent, Response& response) override;


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
		void setAnimationState(AnimationState animationState);
		void setCompletePercentage(float percentage);
		float updatePercentage();
		void onUpdateAnimation(const qsf::JobArguments& jobArguments);
		void cancelMaterialCacheHeating();
		void showStartMenu();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::PipelineStateCompiler& mPipelineStateCompiler;
		qsf::JobProxy				mAnimationJobProxy;	///< Regular animation update job proxy; for updates once a frame
		AnimationState				mAnimationState;
		uint32						mTotalNumberOfCompilerRequests;
		qsf::Stopwatch				mStopwatch;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
