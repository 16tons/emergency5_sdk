// Copyright (C) 2012-2017 Promotion Software GmbH


//[-------------------------------------------------------]
//[ Header guard                                          ]
//[-------------------------------------------------------]
#pragma once


//[-------------------------------------------------------]
//[ Includes                                              ]
//[-------------------------------------------------------]
#include "em5/Export.h"

#include <qsf/log/LogSystem.h>
#include <qsf/base/PropertyHelper.h>
#include <qsf/base/UniqueInstance.h>
#include <qsf/debug/group/DebugGroup.h>
#include <qsf/reflection/type/CampQsfAssetProxy.h>


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
	*    Game debug group implementation
	*/
	class EM5_API_EXPORT GameDebugGroup : public qsf::DebugGroup, public qsf::UniqueInstance<GameDebugGroup>
	{


	//[-------------------------------------------------------]
	//[ Public definitions                                    ]
	//[-------------------------------------------------------]
	public:
		static const uint32 PLUGINABLE_ID;					///< "em5::GameDebugGroup" unique pluginable debug group ID
		static const uint32 SHOW_FREEPLAY_EVENT_OBJECTIVES;	///< "ShowEventDebug" unique class property ID inside the class
		static const uint32 SHOW_HEALTH_STATE;				///< "ShowHealthState" unique class property ID inside the class
		static const uint32 SHOW_CONTAMINATION_RADIUS;		///< "ShowContaminationRadius" unique class property ID inside the class
		static const uint32 SHOW_DEBUG_TEXT_COMPONENT;		///< "ShowDebugTextComponent" unique class property ID inside the class
		static const uint32 SHOW_FIRE_DEBUG;				///< "ShowFireDebug" unique class property ID inside the class
		static const uint32 SHOW_FIRE_RADIUS;				///< "ShowFireRadius" unique class property ID inside the class
		static const uint32 SHOW_FIRE_RECEIVER;				///< "ShowFireReceiver" unique class property ID inside the class
		static const uint32 HIDE_USER_INTERFACE;			///< "HideUserInterface" unique class property ID inside the class
		static const uint32 STARTUP_MAP;					///< "StartupMap" unique class property ID inside the class
		static const uint32 ACTIVE_FREEPLAY;				///< "ActiveFreeplay" unique class property ID inside the class
		static const uint32 ACTIVE_CIVILSPAWN;				///< "ActiveCivilSpawn" unique class property ID inside the class
		static const uint32 SHOW_EVENT_DENSITY;				///< "ShowEventDensity" unique class property ID inside the class
		static const uint32 SHOW_EVENT_FAILCONDITIONS;		///< "ShowEventFailConditions" unique class property ID inside the class
		static const uint32 SHOW_AUTOMATISM_DEBUG;			///< "ShowAutomatismDebug" unique class property ID inside the class
		static const uint32 SHOW_ACTIVITIES_DEBUG;			///< "ShowActivitiesDebug" unique class property ID inside the class
		static const uint32 SHOW_DEBUG_SELECTION_MARKER;	///< "ShowDebugSelectionMarker" unique class property ID inside the class
		static const uint32 SHOW_LOG_SEVERITY_LEVEL;		///< "ShowLogSeverityLevel" unique class property ID inside the class
		static const uint32 ENABLE_DEBUG_INPUT_HANDLER;		///< "EnableDebugInputHandler" unique class property ID inside the class


	//[-------------------------------------------------------]
	//[ Public methods                                        ]
	//[-------------------------------------------------------]
	public:
		/**
		*  @brief
		*    Constructor
		*
		*  @param[in] debugGroupManager
		*    Optional pointer to the debug group manager this debug group should be registered to, can be a null pointer
		*/
		explicit GameDebugGroup(qsf::DebugGroupManager* debugGroupManager);

		/**
		*  @brief
		*    Destructor
		*/
		virtual ~GameDebugGroup();

		/**
		*  @brief
		*    Get the current show health state
		*
		*  @return
		*    The show health state
		*/
		inline bool getShowHealthState() const;

		/**
		*  @brief
		*    Set show health state
		*
		*  @param[in] showHealthState
		*    Show health state
		*/
		void setShowHealthState(bool showHealthState);

		/**
		*  @brief
		*    Get the current show event debug
		*
		*  @return
		*    The show event debug
		*/
		inline bool getShowEventDebug() const;

		/**
		*  @brief
		*    Set the freeplay system active or not
		*/
		void setActiveFreeplay(bool activeFreeplay);

		/**
		*  @brief
		*    Get if the event density debug is shown
		*/
		inline bool getShowEventDensity() const;

		/**
		*  @brief
		*    Set if the event density debug is shown
		*/
		void setShowEventDensity(bool showEventDensity);

		/**
		*  @brief
		*    Get if the event fail conditions debug is shown
		*/
		inline bool getShowEventFailConditions() const;

		/**
		*  @brief
		*    Set if the event fail conditions debug is shown
		*/
		void setShowEventFailConditions(bool showEventFailConditions);

		/**
		*  @brief
		*    Set show event debug
		*
		*  @param[in]
		*    Show event debug text
		*/
		void setShowEventDebug(bool showEventDebug);

		/**
		*  @brief
		*    Get the fire debug display of contamination components
		*/
		inline bool getShowContaminationRadius() const;

		/**
		*  @brief
		*    Set show contamination debug display
		*/
		void setShowContaminationRadius(bool showContaminationRadius);

		/**
		*  @brief
		*    Get show debug text component display
		*/
		inline bool getShowDebugTextComponent() const;

		/**
		*  @brief
		*    Set show debug text component display
		*/
		void setShowDebugTextComponent(bool showDebugTextComponent);

		/**
		*  @brief
		*    Get the fire debug display of fire components
		*
		*  @return
		*    The fire debug display
		*/
		inline bool getShowFireDebug() const;

		/**
		*  @brief
		*    Set show fire debug display
		*
		*  @param[in] showFireDebug
		*    Show fire debug display
		*/
		void setShowFireDebug(bool showFireDebug);

		/**
		*  @brief
		*    Get the fire radius display of fire components
		*
		*  @return
		*    The fire radius value
		*/
		inline bool getShowFireRadius() const;

		/**
		*  @brief
		*    Set show fire radius
		*
		*  @param[in] showFireRadius
		*    Show fire radius values
		*/
		void setShowFireRadius(bool showFireRadius);

		/**
		*  @brief
		*    Get the fire receiver display of fire components
		*
		*  @return
		*    The fire receiver value
		*/
		inline bool getShowFireReceiver() const;

		/**
		*  @brief
		*    Set show fire receiver display
		*
		*  @param[in] showFireReceiver
		*    Show fire receiver display
		*/
		void setShowFireReceiver(bool showFireReceiver);

		/**
		*  @brief
		*    Return "true" if the user interface is hidden
		*/
		inline bool getHideUserInterface() const;

		/**
		*  @brief
		*    Hide or show the user interface
		*/
		void setHideUserInterface(bool hideUserInterface);

		/**
		*  @brief
		*    Get the current startup map asset
		*
		*  @return
		*    The startup map asset
		*/
		inline const qsf::AssetProxy& getStartupMap() const;

		/**
		*  @brief
		*    Set startup map asset
		*
		*  @param[in] startupMap
		*    Startup map asset
		*/
		void setStartupMap(const qsf::AssetProxy& startupMap);

		/**
		*  @brief
		*    Get if the freeplay system is active or not
		*/
		inline bool getActiveFreeplay() const;

		/**
		*  @brief
		*    Get if the automatism debug is shown
		*/
		inline bool getShowAutomatismDebug() const;

		/**
		*  @brief
		*    Set if the automatism debug is shown
		*/
		void setShowAutomatismDebug(bool showAutomatismDebug);

		/**
		*  @brief
		*    Get if the activities debug is shown
		*/
		inline bool getShowActivitiesDebug() const;

		/**
		*  @brief
		*    Set if the activities debug is shown
		*/
		void setShowActivitiesDebug(bool showActivitiesDebug);

		/**
		*  @brief
		*    Get if the debug selection marker is shown
		*/
		inline bool getShowDebugSelectionMarker() const;

		/**
		*  @brief
		*    Set if the debug selection marker is shown
		*/
		void setShowDebugSelectionMarker(bool showDebugSelectionMarker);

		/**
		*  @brief
		*    Get the show log severity level
		*/
		inline qsf::LogMessage::SeverityLevel getShowLogSeverityLevel() const;

		/**
		*  @brief
		*    Set the show log severity level
		*/
		inline void setShowLogSeverityLevel(qsf::LogMessage::SeverityLevel showLogSeverityLevel);

		QSF_DEFINE_SIMPLE_GETTER_AND_SETTER(EnableDebugInputHandler, const std::string&, mEnableDebugInputHandler)


	//[-------------------------------------------------------]
	//[ Private data                                          ]
	//[-------------------------------------------------------]
	private:
		// Connected to the CAMP reflection system
		bool			mShowEventDebug;					///< Show the freeplay event objective status
		bool			mShowEventDensity;					///< Show the event density debug or not
		bool			mShowEventFailConditions;			///< Show the event fail condition debug or not
		bool			mShowHealthState;					///< Show health state of all injured persons?
		bool			mShowContaminationRadius;			///< Show contamination debug radius
		bool			mShowDebugTextComponent;			///< Hide the debug text of the debug text component
		bool			mShowFireDebug;						///< Show the fire debug values of fire components
		bool			mShowFireRadius;					///< Show the fire radius debug display of fire components
		bool			mShowFireReceiver;					///< Show the fire receiver of fire components
		bool			mHideUserInterface;					///< Hide the user interface
		qsf::AssetProxy mStartupMap;						///< Map to use by default on startup
		bool			mShowAutomatismDebug;				///< Show automatism debug or not
		bool			mShowActivitiesDebug;				///< Show activities debug or not
		bool			mShowDebugSelectionMarker;			///< Show debug selection marker or not
		std::string		mEnableDebugInputHandler;			///< Enable debug input handler with the correct string here

		qsf::LogMessage::SeverityLevel mShowLogSeverityLevel;	///< Severity level at which to automatically show the debug log view

		// TODO(mk) Move this to game, this is no debug value
		bool			mActiveFreeplay;			///< Should the generation of freeplay events be active or not?


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
//[ Implementation                                        ]
//[-------------------------------------------------------]
#include "em5/debug/GameDebugGroup-inl.h"


//[-------------------------------------------------------]
//[ CAMP reflection system                                ]
//[-------------------------------------------------------]
QSF_CAMP_TYPE_NONCOPYABLE(em5::GameDebugGroup)
