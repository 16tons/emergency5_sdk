// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include <qsf/gui/GuiDocument.h>


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
	*    EMERGENCY 5 in-game debug window for triggering upgrades
	*/
	class DebugUpgradeBox : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit DebugUpgradeBox(qsf::GuiContext& guiContext);


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5