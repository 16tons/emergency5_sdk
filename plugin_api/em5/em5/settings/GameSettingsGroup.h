// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/settings/SettingsGroup.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/base/UniqueInstance.h>
#include <qsf/time/Time.h>
#include <qsf/reflection/type/CampGlmIVec2.h>


//[-------------------------------------------------------]
//[ Forward declarations                                  ]
//[-------------------------------------------------------]
namespace qsf
{
	class Map;
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
	*    Game settings group implementation
	*/
	class EM5_API_EXPORT GameSettingsGroup : public qsf::SettingsGroup, public qsf::UniqueInstance<GameSettingsGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;	///< "em5::GameSettingsGroup" unique pluginable settings group ID
		static const float  FIXED_FPS;		///< FPS to use for fixed FPS situations (e.g. main menu or video playback)

		// Miscellaneous
		static const uint32 VERSION;								///< "Version" unique class property ID inside the class
		static const uint32 LANGUAGE;								///< "Language" unique class property ID inside the class
		static const uint32 SHOW_VIDEO_SUBTITLES;					///< "ShowVideoSubtitles" unique class property ID inside the class
		static const uint32 FPS_LIMITATION;							///< "FpsLimitation" unique class property ID inside the class
		static const uint32 SCREENSHOT_CAPTURING_SIZE;				///< "ScreenshotCapturingSize" unique class property ID inside the class
		static const uint32 BENCHMARK_ENABLED;						///< "BenchmarkEnabled" unique class property ID inside the class
		static const uint32 SHOW_FREEPLAY_MAP_INTRO_VIDEOS;			///< "ShowFreeplayMapIntroVideos" unique class property ID inside the class
		static const uint32 MATERIAL_CACHE_HEATING_DIALOG_ENABLED;	///< "MaterialCacheHeatingDialogEnabled" unique class property ID inside the class

		// Quality
		static const uint32 USE_MAC_QUALITY;		///< "UseMacQuality"
		static const uint32 OVER_ALL_QUALITY;		///< "OverAllQuality"
		static const uint32 VIDEO_QUALITY;			///< "VideoQuality" unique class property ID inside the class
		static const uint32 WATERJET_QUALITY;		///< "WaterjetQuality" unique class property ID inside the class
		static const uint32 FIRE_HOSE_QUALITY;		///< "FireHoseQuality" unique class property ID inside the class
		static const uint32 POPULATION;				///< "Population" unique class property ID inside the class
		static const uint32 SHADOW_QUALITY;			///< "ShadowQuality" unique class property ID inside the class
		static const uint32 LOD_QUALITY;			///< "LodQuality" unique class property ID inside the class
		static const uint32 EFFECT_QUALITY;			///< "EffectQuality" unique class property ID inside the class

		// Blocked
		static const uint32 INPUT_BLOCKED;					///< "InputBlocked" unique class property ID inside the class
		static const uint32 INPUT_CAMERA_ZOOM_BLOCKED;		///< "InputCameraZoomBlocked" unique class property ID inside the class
		static const uint32 INPUT_CAMERA_ROTATE_BLOCKED;	///< "InputCameraRotateBlocked" unique class property ID inside the class
		static const uint32 INPUT_CAMERA_MOVE_BLOCKED;		///< "InputCameraMoveBlocked" unique class property ID inside the class
		static const uint32 SELECTION_BLOCKED;				///< "SelectionBlocked" unique class property ID inside the class
		static const uint32 MULTI_SELECTION_BLOCKED;		///< "MultiSelectionBlocked" unique class property ID inside the class
		static const uint32 SPECIAL_UNIT_SELECTION_BLOCKED;	///< "SpecialUnitSelectionBlocked" unique class property ID inside the class
		static const uint32 COMMANDS_BLOCKED;				///< "CommandsBlocked" unique class property ID inside the class
		static const uint32 MINIMAP_INPUT_BLOCKED;			///< "MinimapInputBlocked" unique class property ID inside the class
		static const uint32 LEFT_CLICK_BLOCKED;				///< "LeftClickBlocked" unique class property ID inside the class
		static const uint32 ESCAPE_BLOCKED;					///< "EscapeBlocked" unique class property ID inside the class

		// Launcher
		static const uint32 SAVED_ACCOUNTNAME;		///< "SavedAccountname" unique class property ID inside the class
		static const uint32 SAVED_PASSWORD;			///< "SavedAccountPassword" unique class property ID inside the class
		static const uint32 SHOW_LICENSE_TEXT;		///< "ShowLicenseText" unique class property ID inside the class
		static const uint32 SHOW_RATING_REMINDER;	///< "ShowRatingReminder" unique class property ID inside the class
		static const uint32 SHOW_SUPPORT_REMINDER;	///< "ShowSupportReminder" unique class property ID inside the class
		static const uint32 SHOW_NEW_FEATURES_BOX;	///< "ShowNewFeaturesBox" unique class property ID inside the class

		// Gameplay
		static const uint32 DOUBLE_CLICK_REACTION_TIME;				///< "DoubleClickReactionTime" unique class property ID inside the class
		static const uint32 SPAWN_CIVIL_ACTIVE;						///< "SpawnCivilActive" unique class property ID inside the class
		static const uint32 PLAY_TUTORIAL_ACTIVE;					///< "PlayTutorial" unique class property ID inside the class
		static const uint32 TOOLTIP_ACTIVE;							///< "TooltipActive" unique class property ID inside the class
		static const uint32 DRIVE_BACK_WITHOUT_SIRENS_ACTIVE;		///< "DriveBackWithoutSirensActive" unique class property ID inside the class
		static const uint32 BUILDING_CLIPPING_ACTIVE;				///< "BuildingClippingActive" unique class property ID inside the class
		static const uint32 SHINE_THROUGH_ACTIVE;					///< "ShineThroughActive" unique class property ID inside the class
		static const uint32 SHOW_GROUND_MARKERS_FOR_ALL_OWN_UNITS;	///< "ShowGroundMarkersForAllOwnUnits" unique class property ID inside the class
		static const uint32 DIFFICULTY_EM20_MISSIONS;				///< "DifficultyEM20Missions" unique class property ID inside the class

		// Default values
		static const bool MATERIAL_CACHE_HEATING_DIALOG_ENABLED_DEFAULT_VALUE;
		static const bool SHOW_SUPPORT_REMINDER_DEFAULT_VALUE;
		static const bool USE_MAC_QUALITY_DEFAULT_VALUE;

		/**
		*  @brief
		*    Quality setting
		*/
		enum Quality
		{
			LOW_QUALITY	   = 0,	///< Low quality
			MEDIUM_QUALITY = 1,	///< Medium quality
			HIGH_QUALITY   = 2,	///< High quality
			ULTRA_QUALITY  = 3,	///< Ultra quality
			CUSTOM_QUALITY = 4,	///< Custom quality
			DEBUG_QUALITY  = 5	///< Debug quality
		};

		/**
		*  @brief
		*    Video quality setting
		*/
		enum VideoQuality
		{
			VIDEO_DISABLED = 0,	///< Video disabled
			VIDEO_720P     = 1,	///< 720p
			VIDEO_1080P    = 2	///< 1080p
		};

		/**
		*  @brief
		*    The amount of used unit automatism. Eg. the doctor logic has an automatically treatment of injured persons, with can disabled via this setting
		*/
		enum AutomatismValue
		{
			AUTOMATISM_NONE,
			AUTOMATISM_MEDIUM,
			AUTOMATISM_ALL
		};

		/**
		*  @brief
		*    Difficulty setting
		*/
		enum class Difficulty
		{
			UNDEFINED = -1,
			EASY	  = 0,
			NORMAL	  = 1,
			HARD	  = 2
		};

		static const glm::ivec2 DEFAULT_SCREENSHOT_CAPTURING_SIZE;	///< Default screenshot capturing size
		static const bool		DEFAULT_BENCHMARK_ENABLED;
		static const bool		DEFAULT_SHOW_FREEPLAY_MAP_INTRO_VIDEOS;


	//[-------------------------------------------------------]
	//[ Public static methods                                 ]
	//[-------------------------------------------------------]
	public:
		static void setGraphicsSettings(Quality quality);
		static std::string findVideoFilename(const std::string& virtualFilename);
		static void useCustomSettings();


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] settingsGroupManager
		*    Optional pointer to the settings group manager this settings group should be registered to, can be a null pointer
		*/
		explicit GameSettingsGroup(qsf::SettingsGroupManager* settingsGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GameSettingsGroup();

		/**
		*  @brief
		*    Perform settings upgrade
		*/
		void performSettingsUpgrade();

		/**
		*  @brief
		*    Autodetect and set settings for the current hardware
		*/
		void autodetectSettings();

		Quality getAutodetectedGraphicsSetting() const;
		VideoQuality getAutodetectedVideoSetting() const;

		// Not serialized settings, used for customizations via modification projects
		bool getHashSavedAccountPassword() const;
		void setHashSavedAccountPassword(bool hashSavedAccountPassword);
		int getNumberOfTopMipmapsToRemoveOffset() const;
		void setNumberOfTopMipmapsToRemoveOffset(int numberOfTopMipmapsToRemoveOffset);
		bool getDisableModsOnVersionChange() const;
		void setDisableModsOnVersionChange(bool disableModsOnVersionChange);
		bool getPerformContentAuthentication() const;
		void setPerformContentAuthentication(bool performContentAuthentication);
		bool getWaitForReadyMultiplayerPlayers() const;
		void setWaitForReadyMultiplayerPlayers(bool waitForReadyMultiplayerPlayers);
		bool getAlwaysShowObjectiveWindowPlayerColor() const;
		void setAlwaysShowObjectiveWindowPlayerColor(bool alwaysShowObjectiveWindowPlayerColor);
		bool getShowObjectiveWindowEventOnMiniMap() const;
		void setShowObjectiveWindowEventOnMiniMap(bool showObjectiveWindowEventOnMiniMap);
		size_t getScreenNameMaxLengthLimit() const;
		void setScreenNameMaxLengthLimit(size_t screenNameMaxLengthLimit);
		bool getShowMultiplayerClientStarvationPauseSign() const;
		void setShowMultiplayerClientStarvationPauseSign(bool showMultiplayerClientStarvationPauseSign);

		// Miscellaneous
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(Version,								const std::string&, mVersion,							VERSION)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(Language,								const std::string&, mLanguage,							LANGUAGE)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShowVideoSubtitles,					bool,				mShowVideoSubtitles,				SHOW_VIDEO_SUBTITLES)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(FpsLimitation,						float,				mFpsLimitation,						FPS_LIMITATION)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ScreenshotCapturingSize,				glm::ivec2,			mScreenshotCapturingSize,			SCREENSHOT_CAPTURING_SIZE)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(BenchmarkEnabled,						bool,				mBenchmarkEnabled,					BENCHMARK_ENABLED)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShowFreeplayMapIntroVideos,			bool,				mShowFreeplayMapIntroVideos,		SHOW_FREEPLAY_MAP_INTRO_VIDEOS)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(MaterialCacheHeatingDialogEnabled,	bool,				mMaterialCacheHeatingDialogEnabled,	MATERIAL_CACHE_HEATING_DIALOG_ENABLED)

		// Quality
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(UseMacQuality,		bool,		  mUseMacQuality,		USE_MAC_QUALITY)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(OverAllQuality,		Quality,	  mOverAllQuality,		OVER_ALL_QUALITY)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(VideoQuality,			VideoQuality, mVideoQuality,		VIDEO_QUALITY)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(WaterjetQuality,		Quality,	  mWaterjetQuality,		WATERJET_QUALITY)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(FireHoseQuality,		Quality,	  mFireHoseQuality,		FIRE_HOSE_QUALITY)
		inline float getPopulation() const { return mPopulation; }
		inline void setPopulation(float value)
		{
			assignAndPromoteChange(mPopulation, value, POPULATION);
			applyPopulationToMap();
		}
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShadowQuality,		Quality,	  mShadowQuality,		SHADOW_QUALITY)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(LodQuality,			Quality,	  mLodQuality,			LOD_QUALITY)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(EffectQuality,		Quality,	  mEffectQuality,		EFFECT_QUALITY)

		// Blocked
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(InputBlocked,						bool, mInputBlocked,				INPUT_BLOCKED)
			QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(InputCameraZoomBlocked,		bool, mInputCameraZoomBlocked,		INPUT_CAMERA_ZOOM_BLOCKED)
			QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(InputCameraRotateBlocked,		bool, mInputCameraRotateBlocked,	INPUT_CAMERA_ROTATE_BLOCKED)
			QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(InputCameraMoveBlocked,		bool, mInputCameraMoveBlocked,		INPUT_CAMERA_MOVE_BLOCKED)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(SelectionBlocked,					bool, mSelectionBlocked,			SELECTION_BLOCKED)
			QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(MultiSelectionBlocked,		bool, mMultiSelectionBlocked,		MULTI_SELECTION_BLOCKED) // Only a single entity can be selected
			QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(SpecialUnitSelectionBlocked,	bool, mSpecialUnitSelectionBlocked,	SPECIAL_UNIT_SELECTION_BLOCKED) // Unit group handling or any special unit selection is blocked
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(CommandsBlocked,					bool, mCommandsBlocked,				COMMANDS_BLOCKED)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(MinimapInputBlocked,				bool, mMinimapInputBlocked,			MINIMAP_INPUT_BLOCKED)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(LeftClickBlocked,					bool, mLeftClickBlocked,			LEFT_CLICK_BLOCKED)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(EscapeBlocked,					bool, mEscapeBlocked,				ESCAPE_BLOCKED)

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(MouseOverHudElement,				bool, mMouseOverHudElement)
		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(PlayUnitHints,						bool, mPlayUnitHints)

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(CutsceneRunning,					bool, mCutsceneRunning)

		// Launcher
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(SavedAccountname,		std::string, mAccountName,		   SAVED_ACCOUNTNAME)	// TODO(co) Fix the spelling mismatch, "SavedAccountname" should be "SavedAccountName" to be consistent (game is already out, so might not be a good idea to change it)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(SavedAccountPassword,	std::string, mAccountPassword,	   SAVED_PASSWORD)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShowLicenseText,		bool,		 mShowLicenseText,	   SHOW_LICENSE_TEXT)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShowRatingReminder,	bool,		 mShowRatingReminder,  SHOW_RATING_REMINDER)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShowSupportReminder,	bool,		 mShowSupportReminder, SHOW_SUPPORT_REMINDER)

		// Main menu
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShowNewFeaturesBox,	bool,		 mShowNewFeaturesBox, SHOW_NEW_FEATURES_BOX)

		// Gameplay settings
		AutomatismValue getAutomatismValue() const;
		void setAutomatismValue(AutomatismValue automatismValue);

		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(DoubleClickReactionTime,	qsf::Time,	mDoubleClickReactionTime,	DOUBLE_CLICK_REACTION_TIME)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(SpawnCivilActive,			bool,		mSpawnCivilActive,			SPAWN_CIVIL_ACTIVE)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(PlayTutorialActive,		bool,		mPlayTutorialActive,		PLAY_TUTORIAL_ACTIVE)

		bool getTooltipActive() const;
		void setTooltipActive(bool active);
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(DriveBackWithoutSirens,			bool,	mDriveBackWithoutSirens,			DRIVE_BACK_WITHOUT_SIRENS_ACTIVE)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(BuildingClippingActive,			bool,	mBuildingClippingActive,			BUILDING_CLIPPING_ACTIVE)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShineThroughActive,				bool,	mShineThroughActive,				SHINE_THROUGH_ACTIVE)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(ShowGroundMarkersForAllOwnUnits,	bool,	mShowGroundMarkersForAllOwnUnits,	SHOW_GROUND_MARKERS_FOR_ALL_OWN_UNITS)
		QSF_DEFINE_PROPERTY_GETTER_AND_SETTER(DifficultyEM20Missions,			Difficulty,	mDifficultyEM20Missions,		DIFFICULTY_EM20_MISSIONS)

		void applyPopulationToMap() const;


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		std::string mVersion;
		std::string mLanguage;					///< Chosen language to use for texts, audio speech, etc.; this is a short identifier like "de", "en", "fr"
		bool		mShowVideoSubtitles;
		float		mFpsLimitation;
		glm::ivec2	mScreenshotCapturingSize;	///< Screenshot capturing size. Examples 1920x1080 (1080p, full HD), 2048x1536 (2000p), 3840x2160 (2160p), 4520x2540 (2540p), 4096x3072 (4000p), 7680x4320 (4320p).
		bool		mBenchmarkEnabled;
		bool		mShowFreeplayMapIntroVideos;
		bool		mMaterialCacheHeatingDialogEnabled;

		// Not serialized settings, used for customizations via modification projects
		bool		mHashSavedAccountPassword;
		int			mNumberOfTopMipmapsToRemoveOffset;
		bool		mDisableModsOnVersionChange;
		bool		mPerformContentAuthentication;
		bool		mWaitForReadyMultiplayerPlayers;
		bool		mAlwaysShowObjectiveWindowPlayerColor;
		bool		mShowObjectiveWindowEventOnMiniMap;
		size_t		mScreenNameMaxLengthLimit;
		bool		mShowMultiplayerClientStarvationPauseSign;

		// Quality
		bool		 mUseMacQuality;
		Quality		 mOverAllQuality;
		VideoQuality mVideoQuality;
		Quality		 mWaterjetQuality;
		Quality		 mFireHoseQuality;
		float		 mPopulation;
		Quality		 mShadowQuality;
		Quality		 mLodQuality;
		Quality		 mEffectQuality;

		// Blocked
		bool	mInputBlocked;
		bool	mInputCameraZoomBlocked;
		bool	mInputCameraRotateBlocked;
		bool	mInputCameraMoveBlocked;
		bool	mSelectionBlocked;
		bool	mMultiSelectionBlocked;
		bool	mSpecialUnitSelectionBlocked;
		bool	mCommandsBlocked;
		bool	mMinimapInputBlocked;
		bool	mLeftClickBlocked;
		bool	mEscapeBlocked;
		bool	mMouseOverHudElement;	///< Indicates if the mouse cursor is over an HUD element, which isn't marked as "invisible" to mouse input
		bool	mPlayUnitHints;			///< Should unit hints be played?
		bool	mCutsceneRunning;

		// Launcher
		std::string mAccountName;			///< Saved account name from last login
		std::string mAccountPassword;		///< Saved account password from last login
		bool		mShowLicenseText;		///< Show the license text at program start?
		bool		mShowRatingReminder;	///< Show the rating reminder after a while?
		bool		mShowSupportReminder;	///< Show the support reminder after a while?

		// Main menu
		bool		mShowNewFeaturesBox;	///< Show the info box about new features for EM2017

		// Gameplay settings
		AutomatismValue mAutomatismValue;
		qsf::Time		mDoubleClickReactionTime;
		bool			mSpawnCivilActive;
		bool			mPlayTutorialActive;		///< Indicates if the tutorial should be started on Munich map start
		bool			mTooltipActive;
		bool			mDriveBackWithoutSirens;
		bool			mBuildingClippingActive;
		bool			mShineThroughActive;
		bool			mShowGroundMarkersForAllOwnUnits;
		Difficulty		mDifficultyEM20Missions;


	//[-------------------------------------------------------]
	//[ CAMP reflection system                                ]
	//[-------------------------------------------------------]
	QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


	};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
} // em5


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GameSettingsGroup)
QSF_CAMP_TYPE_NONCOPYABLE(em5::GameSettingsGroup::Quality)
QSF_CAMP_TYPE_NONCOPYABLE(em5::GameSettingsGroup::VideoQuality)
QSF_CAMP_TYPE_NONCOPYABLE(em5::GameSettingsGroup::AutomatismValue)
QSF_CAMP_TYPE_NONCOPYABLE(em5::GameSettingsGroup::Difficulty)
