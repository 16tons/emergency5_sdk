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
#include <qsf/message/MessageProxy.h>
#include <qsf/job/JobProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	namespace multiplayer
	{
		struct GameModeUpdateData;
	}
}

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
	*    EMERGENCY 5 in-game information bar
	*/
	class EM5_API_EXPORT MenuBar : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit MenuBar(qsf::GuiContext& guiContext);
		~MenuBar();

		void rebuildGui();
		void openBuyUpgradeWindow();
		void showPauseSign(bool showPauseSign);
		bool getAllowGameSpeedChanges() const;
		void setAllowGameSpeedChanges(bool allowGameSpeedChanges);
		void clickSpeed0();
		void clickSpeed1();
		void clickSpeed2();
		void clickSpeed3();

		void updateBuyableButtonVisibility(bool visible = true);
		void showBuyableBlinkText();
		void hideBuyableBlinkText();

		void setShowCreditsInsteadOfScore(bool showCreditsInsteadOfScore);
		void setCreditsScoreVisible(bool visible);
		void setBuyButtonVisible(bool visible);

		void setCountdownTimer(qsf::Time countDownTime);
		void showCountdownTimerElement(bool show);


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
		void onCreditsChanged();
		void onScoreChanged();
		void onGameSpeedChange(const qsf::MessageParameters& parameters);
		void onFreeplayEventLevelChanged(const qsf::MessageParameters& parameters);
		void updateTimeOfDay();
		void changeGameSpeed(float gameSpeed);
		void onGameModeDataChanged(const multiplayer::GameModeUpdateData& gameModeData);
		void onUpdateBlink(const qsf::JobArguments& jobArguments);
		void updateSpeedButtons(const Rocket::Core::Element* pressedButtonElement);
		void onCreditsCounting(const qsf::JobArguments& jobArguments);
		void onPointsCounting(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		bool				mAllowGameSpeedChanges;
		float				mBlinkPhase;
		qsf::MessageProxy	mMessageProxyGameSpeed;
		qsf::MessageProxy	mMessageProxyCredits;	///< Message proxy for the player credits
		qsf::MessageProxy	mMessageProxyScore;		///< Message proxy for the player score
		qsf::MessageProxy	mMessageProxyEventLevel;	///< Message proxy for the freeplay event level
		qsf::JobProxy		mUpdateJobProxy;		///< Job proxy
		qsf::JobProxy		mBlinkJobProxy;			///< Job proxy
		qsf::JobProxy		mAnimationJobProxyCredits;
		qsf::JobProxy		mAnimationJobProxyPoints;
		uint32				mStartCountingPointsValue;
		uint32				mStartCountingCreditsValue;
		uint32				mEndCountingPointsValue;
		uint32				mEndCountingCreditsValue;
		bool				mShowCreditsInsteadOfScore;
		bool				mCreditsScoreVisible;
		bool				mBuyButtonVisible;
		bool				mTimeOfDayMode;
		std::string			mDisplayedTimeOfDay;	///< Last displayed time of day
		std::string			mDisplayedFreeplayTime;	///< Last displayed freeplay time
		qsf::Time			mOldCountDownTime;

		// TODO(sw) This is an fix for the crash when going from the game to the main menu and initiate then a gui hot reloading -> The connection to the menubar was still there but the menubar instance was destroyed
		// Proper fix see QSF Bug "H301 GUI | "qsf::GuiDocument" | We need to guarantee that for each e.g. "onPostShow()" there's a "onPostHide()""
		boost::signals2::connection	mReloadGuiConnection;

		// EM20 specific
		Rocket::Core::Element* mMissionNameElement;
		Rocket::Core::ElementText* mMissionNameTextElement;

		Rocket::Core::Element* mMissionDifficultyElement;
		Rocket::Core::ElementText* mMissionDifficultyTextElement;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
