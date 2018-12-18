// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/gui/GuiDocument.h>
#include <qsf/message/MessageProxy.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Entity;
}
namespace em5
{
	class GameComponent;
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
	*    EMERGENCY 5 in-game selection window
	*/
	class EM5_API_EXPORT SelectionWindowCommandable : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit SelectionWindowCommandable(qsf::GuiContext& guiContext);
		virtual ~SelectionWindowCommandable();
		bool isCommandBarVisible() const;
		void setCommandBarVisible(bool visible);
		bool areSkillsVisible() const;
		void setSkillsVisible(bool visible);


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
		void setupGui();
		void onReloadGui();
		void onSelectionChange(const qsf::MessageParameters& parameters = qsf::MessageParameters());

		void updateInformationRegular();
		void updateCommandBarButtonVisibility() const;
		void updateCommandBarVisibility(bool visible = true) const;
		void updateSkillsVisibility();
		void updateEnergyInformation();
		void updateCareInformation();
		void updateEquipmentInformation();
		void updateCommandBarState(qsf::Entity& selectedEntity);
		void refreshCameraFollowButton(const qsf::MessageParameters& parameters);


	//[-------------------------------------------------------]
	//[ Private definitions                                   ]
	//[-------------------------------------------------------]
	private:
		static const char HEALTHBAR_COLOR_NORMAL[];
		static const char HEALTHBAR_COLOR_INJURED[];


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				mCommandBarVisible;
		bool				mCommandBarToggleState;
		bool				mSkillsVisible;
		ProgressBar*		mProgressBar;
		GameComponent*		mGameComponent;

		qsf::MessageProxy	mMessageProxy;
		qsf::MessageProxy	mFollowCameraEntityChangedProxy;
		qsf::JobProxy		mUpdateJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
