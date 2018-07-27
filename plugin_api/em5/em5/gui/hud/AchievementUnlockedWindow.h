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
#include <qsf/message/MessageProxy.h>
#include <qsf/audio/AudioProxy.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace em5
{
	class Achievement;
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
	*    Achievement unlocked message GUI window
	*/
	class AchievementUnlockedWindow : public qsf::GuiDocument
	{


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*/
		explicit AchievementUnlockedWindow(qsf::GuiContext& guiContext);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~AchievementUnlockedWindow();


	//[-------------------------------------------------------]
	//[ Public virtual qsf::GuiDocument methods               ]
	//[-------------------------------------------------------]
	public:
		virtual void processEvent(const std::string& eventKey, Rocket::Core::Event& event) override;


	//[-------------------------------------------------------]
	//[ Private methods                                       ]
	//[-------------------------------------------------------]
	private:
		void onUpdateAnimation(const qsf::JobArguments& jobArguments);
		void onUpdateBackup(const qsf::JobArguments& jobArguments);

		void showAchievement(const Achievement* achievement);
		void onUnlockedAchievement(const qsf::MessageParameters& parameters);
		bool canPlayNow();


	//[-------------------------------------------------------]
	//[ Private declarations                                  ]
	//[-------------------------------------------------------]
	private:
		// Animation states
		enum AnimationStates
		{
			DRIVE_IN,
			STAND,
			DRIVE_OUT
		};


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		qsf::Time			mAnimationTime;				///< Holds the past animation time
		AnimationStates		mAnimationState;			///< Holds the current animation state

		qsf::MessageProxy	mAchievementMessageProxy;	///< This proxy trigger the GUI show function and starts the animation
		qsf::JobProxy		mAnimationJobProxy;			///< Regular animation update job proxy; for updates once a frame

		qsf::AudioProxy		mAudioProxy;

		// Backup
		std::list<const Achievement*> mAchievementList;		///< If many achievements trigger at the same time or the user is in a cutscene, queue them up
		qsf::JobProxy		mBackupAchievementJobProxy;


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5
