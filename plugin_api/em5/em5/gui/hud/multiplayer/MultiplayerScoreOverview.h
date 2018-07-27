// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>
#include <qsf/math/Color3.h>
#include <qsf/job/JobProxy.h>


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
	*    EMERGENCY 5 in-game debug window for triggering special "actions", i.e. add player credits, add player score
	*/
	class MultiplayerScoreOverview : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MultiplayerScoreOverview(qsf::GuiContext& guiContext);
		virtual ~MultiplayerScoreOverview();


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
		void updatePlayerScore();


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy	mUpdateJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
