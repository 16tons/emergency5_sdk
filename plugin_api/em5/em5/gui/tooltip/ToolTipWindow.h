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
namespace em5
{
	class ToolTipManager;
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
	*    EMERGENCY 5 tooltip window
	*/
	class ToolTipWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Friend definitions                                    ]
	//[-------------------------------------------------------]
		friend class ToolTipManager; // Needs access to the reload GUI method


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		explicit ToolTipWindow(qsf::GuiContext& guiContext);
		virtual ~ToolTipWindow();

		void updateText(const std::string& text, const glm::ivec2& mousePosition);
		void updatePosition(const glm::ivec2& mousePosition);

		void activate();
		void deactivate(bool withFadeOut);

		// Configuration
		bool isFadeOutEnabled() const;
		void enabledFadeOut(bool enable);

		float getFadeOutTimeInSeconds() const;
		void setFadeOutTimeInSeconds(float fadeTime);

		bool isShowDelayEnabled() const;
		void enabledShowDelay(bool enable);

		float getShowDelayInSeconds() const;
		void setShowDelayInSeconds(float delayTime);


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void setupUI();
		void onReloadGui();
		glm::vec2 getContentSize(Rocket::Core::Element& element);

		void startShowDelay();
		void startFadeOut();
		void activateJob();
		void updateAnimation(const qsf::JobArguments& jobArguments);


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::JobProxy				mUpdateJobProxy;
		Rocket::Core::Element*		mToolTipElement;
		Rocket::Core::ElementText*	mToolTipTextElement;
		Rocket::Core::Element*		mToolTipFormattedElement;
		glm::vec2					mCurrentContentSize;
		glm::vec2					mCurrentTooltipSize;

		// Show delay / fade out handling
		bool	mShowWithDelayEnabled;
		bool	mShowWithDelay;
		float	mShowDelayInSeconds;
		float	mCurrentShowDelayInSeconds;

		bool	mFadingOutEnabled;
		bool	mFadingOut;
		float	mFadeOutTimeInSeconds;
		float	mCurrentFadeOutTimeInSeconds;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
