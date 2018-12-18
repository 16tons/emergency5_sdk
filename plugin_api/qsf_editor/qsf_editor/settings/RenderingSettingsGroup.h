// Copyright (C) 2012-2018 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "qsf_editor/application/manager/CameraManager.h"

#include <qsf/base/UniqueInstance.h>
#include <qsf/settings/SettingsGroup.h>
#include <qsf/reflection/type/CampGlmIVec2.h>


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
namespace qsf
{
	namespace editor
	{


		//[-------------------------------------------------------]
		//[ Classes                                               ]
		//[-------------------------------------------------------]
		/**
		*  @brief
		*    QSF editor rendering settings group implementation
		*/
		class QSF_EDITOR_API_EXPORT RenderingSettingsGroup : public SettingsGroup, public qsf::UniqueInstance<RenderingSettingsGroup>
		{


		//[-------------------------------------------------------]
		//[ Public definitions                                    ]
		//[-------------------------------------------------------]
		public:
			static const uint32 PLUGINABLE_ID;							///< "qsf::editor::RenderingSettingsGroup" unique pluginable settings group ID
			static const uint32 FPS_LIMIT;								///< "FpsLimit" unique class property ID inside the class
			static const uint32 CAMERA_MODE;							///< "CameraMode" unique class property ID inside the class
			static const uint32 CAMERA_MOVEMENT_SPEED_FACTOR;			///< "CameraMovementSpeedFactor" unique class property ID inside the class
			static const uint32 VISUAL_HIGHLIGHTING;					///< "VisualHighlighting" unique class property ID inside the class
			static const uint32 SHOW_ICONS;								///< "ShowIcons" unique class property ID inside the class
			static const uint32 SHOW_COMPASS;							///< "ShowCompass" unique class property ID inside the class
			static const uint32 SCREENSHOT_CAPTURING_SIZE;				///< "ScreenshotCapturingSize" unique class property ID inside the class
			static const uint32 HDR_SCREENSHOT_CAPTURING;				///< "HdrScreenshotCapturing" unique class property ID inside the class
			static const glm::ivec2 DEFAULT_SCREENSHOT_CAPTURING_SIZE;	///< Default screenshot capturing size


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
			inline explicit RenderingSettingsGroup(SettingsGroupManager* settingsGroupManager);

			/**
			*  @brief
			*    Destructor
			*/
			inline virtual ~RenderingSettingsGroup();

			/** Return the FPS limit */
			inline float getFpsLimit() const;

			/** Set the FPS limit */
			inline void setFpsLimit(float fpsLimit);

			/**
			*  @brief
			*    Get the current camera mode
			*
			*  @return
			*    The camera mode
			*/
			inline CameraManager::CameraMode getCameraMode() const;

			/**
			*  @brief
			*    Set camera mode
			*
			*  @param[in] mode
			*    New camera mode to set
			*/
			inline void setCameraMode(CameraManager::CameraMode mode);

			/**
			*  @brief
			*    Get the current camera movement speed factor
			*
			*  @return
			*    The camera movement speed factor
			*/
			inline float getCameraMovementSpeedFactor() const;

			/**
			*  @brief
			*    Set camera movement speed factor
			*
			*  @param[in] speedFactor
			*    New camera movement speed factor
			*/
			inline void setCameraMovementSpeedFactor(float speedFactor);

			/**
			*  @brief
			*    Get whether or not visual highlighting is enabled
			*
			*  @return
			*    "true" if visual highlighting is enabled, else "false"
			*/
			inline bool getVisualHighlighting() const;

			/**
			*  @brief
			*    Set whether or not visual highlighting is enabled
			*
			*  @param[in] visualHighlighting
			*    "true" if visual highlighting is enabled, else "false"
			*/
			inline void setVisualHighlighting(bool visualHighlighting);

			/** Return if the user can see icons in the world */
			inline bool getShowIcons() const;

			/** Set if the user can see icons in the world */
			inline void setShowIcons(bool showIcons);

			/** Return whether the compass is shown */
			inline bool getShowCompass() const;

			/** Set if the compass is shown */
			inline void setShowCompass(bool showCompass);

			/**
			*  @brief
			*    Get the screenshot capturing size
			*
			*  @return
			*    Screenshot capturing size, do not destroy the instance
			*/
			inline const glm::ivec2& getScreenshotCapturingSize() const;

			/**
			*  @brief
			*    Set screenshot capturing size
			*
			*  @param[in] screenshotCapturingSize
			*    Screenshot capturing size
			*/
			inline void setScreenshotCapturingSize(const glm::ivec2& screenshotCapturingSize);

			/**
			*  @brief
			*    Return whether or not HDR screenshot capturing is enabled
			*
			*  @return
			*    "true" if HDR screenshot capturing is enabled, else "false"
			*/
			inline bool getHdrScreenshotCapturing() const;

			/**
			*  @brief
			*    Set whether or not HDR screenshot capturing is enabled
			*
			*  @param[in] hdrScreenshotCapturing
			*    "true" if HDR screenshot capturing is enabled, else "false"
			*/
			inline void setHdrScreenshotCapturing(bool hdrScreenshotCapturing);


		//[-------------------------------------------------------]
		//[ Private data                                          ]
		//[-------------------------------------------------------]
		private:
			// Connected to the CAMP reflection system
			float						mFpsLimit;					///< FPS limit
			CameraManager::CameraMode	mCameraMode;				///< Camera mode
			float						mCameraMoveSpeedFactor;		///< Moving speed factor of the camera
			bool						mVisualHighlighting;		///< "true" if visual highlighting is enabled, else "false"
			bool						mShowIcons;					///< Show icons in the editor
			bool						mShowCompass;				///< Show a compass visualization in the editor
			glm::ivec2					mScreenshotCapturingSize;	///< Screenshot capturing size
			bool						mHdrScreenshotCapturing;	///< "true" if HDR screenshot capturing is enabled, else "false"


		//[-------------------------------------------------------]
		//[ CAMP reflection system                                ]
		//[-------------------------------------------------------]
		QSF_CAMP_RTTI()	// Only adds the virtual method "campClassId()", nothing more


		};


//[-------------------------------------------------------]
//[ Namespace                                             ]
//[-------------------------------------------------------]
	} // editor
} // qsf


//[-------------------------------------------------------]
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "qsf_editor/settings/RenderingSettingsGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(qsf::editor::RenderingSettingsGroup)
